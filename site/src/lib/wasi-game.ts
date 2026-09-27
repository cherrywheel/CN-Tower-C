// runs the real wasm32-wasi build of the game in the browser
//
// the game reads stdin with blocking calls and the browser cant block
// so every command reruns the game from the start with all commands so far
// and stops it at the first read that finds no more input
// the game is deterministic with a fixed CN_TOWER_SEED and a fixed clock so each rerun lands in the same place
// saves live in memory and get rebuilt by the rerun too

const ESUCCESS = 0
const EBADF = 8
const EINVAL = 28
const ENOENT = 44

const FILETYPE_CHAR = 2
const FILETYPE_DIR = 3
const FILETYPE_FILE = 4

const PREOPEN_FD = 3
const ALL_RIGHTS = 0xffffffffffffffffn

class NeedInput {}
class Exit {
  code: number
  constructor(code: number) {
    this.code = code
  }
}

export interface RunResult {
  // what a terminal would show, game output with the typed commands echoed
  screen: string
  // true when the game quit on its own
  exited: boolean
}

type OpenFile = { name: string; pos: number; append: boolean }

export class WasiGame {
  private module: WebAssembly.Module
  private seed: string

  private constructor(module: WebAssembly.Module, seed: string) {
    this.module = module
    this.seed = seed
  }

  static async load(url: string): Promise<WasiGame> {
    const res = await fetch(url)
    if (!res.ok) throw new Error(`cant load ${url}: ${res.status}`)
    const module = await WebAssembly.compile(await res.arrayBuffer())
    return new WasiGame(module, String(Math.floor(Math.random() * 1e9)))
  }

  newSeed() {
    this.seed = String(Math.floor(Math.random() * 1e9))
  }

  setSeed(seed: string) {
    this.seed = seed
  }

  run(lines: string[]): RunResult {
    const enc = new TextEncoder()
    const dec = new TextDecoder()
    const stdin = enc.encode(lines.map((l) => l + '\n').join(''))
    let stdinPos = 0
    let screen = ''
    const files = new Map<string, Uint8Array>()
    const fds = new Map<number, OpenFile>()
    let nextFd = PREOPEN_FD + 1
    let memory: WebAssembly.Memory

    const view = () => new DataView(memory.buffer)
    const bytes = () => new Uint8Array(memory.buffer)
    const str = (ptr: number, len: number) => dec.decode(bytes().slice(ptr, ptr + len))

    const iovecs = (ptr: number, len: number) => {
      const out: Uint8Array[] = []
      for (let i = 0; i < len; i++) {
        const buf = view().getUint32(ptr + i * 8, true)
        const n = view().getUint32(ptr + i * 8 + 4, true)
        out.push(new Uint8Array(memory.buffer, buf, n))
      }
      return out
    }

    const strings = (list: string[], ptrs: number, buf: number) => {
      let p = buf
      list.forEach((s, i) => {
        const b = enc.encode(s + '\0')
        view().setUint32(ptrs + i * 4, p, true)
        bytes().set(b, p)
        p += b.length
      })
      return ESUCCESS
    }
    const sizes = (list: string[], countPtr: number, sizePtr: number) => {
      view().setUint32(countPtr, list.length, true)
      view().setUint32(sizePtr, list.reduce((n, s) => n + enc.encode(s).length + 1, 0), true)
      return ESUCCESS
    }

    const args = ['cn_tower_game']
    const env = [`CN_TOWER_SEED=${this.seed}`, 'CN_TOWER_PLAIN=1']

    const wasi = {
      args_get: (a: number, b: number) => strings(args, a, b),
      args_sizes_get: (a: number, b: number) => sizes(args, a, b),
      environ_get: (a: number, b: number) => strings(env, a, b),
      environ_sizes_get: (a: number, b: number) => sizes(env, a, b),

      clock_time_get: (_id: number, _precision: bigint, ptr: number) => {
        view().setBigUint64(ptr, 1_700_000_000_000_000_000n, true)
        return ESUCCESS
      },

      fd_fdstat_get: (fd: number, ptr: number) => {
        let type
        if (fd <= 2) type = FILETYPE_CHAR
        else if (fd === PREOPEN_FD) type = FILETYPE_DIR
        else if (fds.has(fd)) type = FILETYPE_FILE
        else return EBADF
        bytes().fill(0, ptr, ptr + 24)
        view().setUint8(ptr, type)
        // full rights on stdio make isatty false so the game stays in plain mode
        view().setBigUint64(ptr + 8, ALL_RIGHTS, true)
        view().setBigUint64(ptr + 16, ALL_RIGHTS, true)
        return ESUCCESS
      },
      fd_fdstat_set_flags: () => ESUCCESS,

      fd_prestat_get: (fd: number, ptr: number) => {
        if (fd !== PREOPEN_FD) return EBADF
        view().setUint8(ptr, 0)
        view().setUint32(ptr + 4, 1, true)
        return ESUCCESS
      },
      fd_prestat_dir_name: (fd: number, ptr: number) => {
        if (fd !== PREOPEN_FD) return EBADF
        bytes().set(enc.encode('.'), ptr)
        return ESUCCESS
      },

      fd_read: (fd: number, iovs: number, len: number, nread: number) => {
        let total = 0
        if (fd === 0) {
          if (stdinPos >= stdin.length) throw new NeedInput()
          for (const iov of iovecs(iovs, len)) {
            // hand over one line at a time so the echo lands right after the prompt
            let n = 0
            while (n < iov.length && stdinPos < stdin.length) {
              const b = stdin[stdinPos++]
              iov[n++] = b
              if (b === 10) break
            }
            screen += dec.decode(iov.slice(0, n))
            total += n
            if (n && iov[n - 1] === 10) break
          }
        } else {
          const f = fds.get(fd)
          if (!f) return EBADF
          const data = files.get(f.name) ?? new Uint8Array()
          for (const iov of iovecs(iovs, len)) {
            const chunk = data.subarray(f.pos, f.pos + iov.length)
            iov.set(chunk)
            f.pos += chunk.length
            total += chunk.length
            if (chunk.length < iov.length) break
          }
        }
        view().setUint32(nread, total, true)
        return ESUCCESS
      },

      fd_write: (fd: number, iovs: number, len: number, nwritten: number) => {
        let total = 0
        for (const iov of iovecs(iovs, len)) {
          if (fd === 1 || fd === 2) {
            screen += dec.decode(iov)
          } else {
            const f = fds.get(fd)
            if (!f) return EBADF
            const old = files.get(f.name) ?? new Uint8Array()
            const at = f.append ? old.length : f.pos
            const next = new Uint8Array(Math.max(old.length, at + iov.length))
            next.set(old)
            next.set(iov, at)
            files.set(f.name, next)
            f.pos = at + iov.length
          }
          total += iov.length
        }
        view().setUint32(nwritten, total, true)
        return ESUCCESS
      },

      fd_seek: (fd: number, offset: bigint, whence: number, ptr: number) => {
        const f = fds.get(fd)
        if (!f) return fd <= 2 ? ESUCCESS : EBADF
        const size = files.get(f.name)?.length ?? 0
        const base = whence === 0 ? 0 : whence === 1 ? f.pos : size
        const pos = base + Number(offset)
        if (pos < 0) return EINVAL
        f.pos = pos
        view().setBigUint64(ptr, BigInt(pos), true)
        return ESUCCESS
      },

      fd_close: (fd: number) => (fds.delete(fd) ? ESUCCESS : fd <= PREOPEN_FD ? ESUCCESS : EBADF),

      path_open: (
        dirfd: number, _dirflags: number, pathPtr: number, pathLen: number, oflags: number,
        _rb: bigint, _ri: bigint, fdflags: number, fdPtr: number,
      ) => {
        if (dirfd !== PREOPEN_FD) return EBADF
        const name = str(pathPtr, pathLen).replace(/^\.\//, '')
        const exists = files.has(name)
        if (!exists && !(oflags & 1)) return ENOENT
        if (!exists || oflags & 8) files.set(name, new Uint8Array())
        const fd = nextFd++
        fds.set(fd, { name, pos: 0, append: !!(fdflags & 1) })
        view().setUint32(fdPtr, fd, true)
        return ESUCCESS
      },

      path_filestat_get: (dirfd: number, _flags: number, pathPtr: number, pathLen: number, ptr: number) => {
        if (dirfd !== PREOPEN_FD) return EBADF
        const name = str(pathPtr, pathLen).replace(/^\.\//, '')
        const data = files.get(name)
        if (!data) return ENOENT
        bytes().fill(0, ptr, ptr + 64)
        view().setUint8(ptr + 16, FILETYPE_FILE)
        view().setBigUint64(ptr + 24, 1n, true)
        view().setBigUint64(ptr + 32, BigInt(data.length), true)
        return ESUCCESS
      },

      proc_exit: (code: number) => {
        throw new Exit(code)
      },
    }

    const instance = new WebAssembly.Instance(this.module, { wasi_snapshot_preview1: wasi })
    memory = instance.exports.memory as WebAssembly.Memory
    try {
      ;(instance.exports._start as () => void)()
      return { screen, exited: true }
    } catch (e) {
      if (e instanceof NeedInput) return { screen, exited: false }
      if (e instanceof Exit) return { screen, exited: true }
      throw e
    }
  }
}
