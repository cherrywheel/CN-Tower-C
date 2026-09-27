<p align="center">
  <img src="assets/social-preview.png" alt="CN Tower: a text adventure in C">
</p>

# CN Tower

A text adventure about getting to the top of Toronto's CN Tower — and doing the EdgeWalk.
Rewritten in C from the [Python original](https://github.com/cherrywheel/CN-Tower):
no dependencies, no internet, one binary.

## Download

Grab a build from [Releases](../../releases). Every push to `main` ships a new one.

| OS | Architecture | File |
|---|---|---|
| Windows | x64 | `cn_tower_game-windows-x64.exe` |
| Windows | x86 | `cn_tower_game-windows-x86.exe` |
| Windows | ARM64 | `cn_tower_game-windows-arm64.exe` |
| macOS 11+ | Apple Silicon + Intel | `cn_tower_game-macos-universal.tar.gz` |
| Linux | x86_64 | `cn_tower_game-linux-x86_64.tar.gz` |
| Linux | ARM64 / Baikal-M | `cn_tower_game-linux-aarch64.tar.gz` |
| Linux | ARMv7 | `cn_tower_game-linux-armhf.tar.gz` |
| Linux | x86 | `cn_tower_game-linux-i686.tar.gz` |
| Linux | RISC-V 64 | `cn_tower_game-linux-riscv64.tar.gz` |
| Linux | MIPS32 LE / Baikal-T1 | `cn_tower_game-linux-mipsel.tar.gz` |

Linux builds are static and run on any distro. In CI every build plays the
game through to the win — the non-x86 Linux ones under QEMU.

Windows on ARM: I couldn't care less about it, but it's there. It's the one
build CI only compiles and never runs.

macOS builds aren't signed, so clear the quarantine flag first:

```
xattr -d com.apple.quarantine cn_tower_game
```

## Build

Windows, from a Developer Command Prompt:

```
cd src
nmake
```

Linux, macOS, BSD:

```
cd src
make
```

Plain C99 + POSIX (WinAPI on Windows). If it has a C compiler, it builds.

### Elbrus

Purely for fun. No prebuilt binary and no CI — MCST doesn't hand out `lcc`.
On the real thing:

```
cd src
make CC=lcc
```

## Play

Type commands like `Go North`, `Buy Ticket`, `Look Around`. Case doesn't matter.

| Command | What it does |
|---|---|
| `Help` | List commands |
| `Look` | Describe where you are again |
| `Inventory` | Money and items |
| `Save` / `Load` | Save and load the game |
| `Restart` / `Exit` | Start over / quit |
| `Debug` | Money, items, teleport, Sweet+ mode |

One good ending, several bad ones.

Run from `src`, the game keeps saves in `../data/`. Run from anywhere else,
it keeps them in the current directory.

## Interface

In a terminal the game goes full screen: location, money and items up top,
the story in the middle, hints at the bottom — only the actions that work
right now, plus the general commands.

| Key | Action |
|---|---|
| `Tab` | Complete a command, press again to cycle |
| `→` | Accept the grey suggestion |
| `↑` / `↓` | Command history |
| `Ctrl+U` | Clear the line |
| `Ctrl+D` / `Ctrl+C` | Quit |

`--plain` or `CN_TOWER_PLAIN=1` gives you the classic line-by-line mode.
It also kicks in by itself when output isn't a terminal.

## Changes from the Python version

* No IP-based country check. Sweet+ mode lives in the debug menu.
* Dialogue and ASCII art are built in.
* The EdgeWalk is actually winnable now. Just a Chill Guy is east of the
  Glass Floor, Alex's phone can be returned at the Info Booth for $100
  (`Return Phone`), the LookOut has an elevator back down (`Go Back`), and
  Alex can be met a second time.

## Art

`assets/` holds the artwork: `social-preview.png` (this README and the repo
preview), `cn_tower.ico` (embedded into the Windows `.exe`) and a few
alternatives. The repo preview is set by hand under
Settings → General → Social preview.
