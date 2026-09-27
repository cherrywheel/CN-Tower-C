<p align="center">
  <img src="assets/social-preview.png" alt="cn tower text adventure in c">
</p>

# cn tower

a text adventure about climbing torontos cn tower and doing the edgewalk

rewritten in c from the [python original](https://github.com/cherrywheel/CN-Tower)

no dependencies no internet just one binary

## download

grab a build from [releases](../../releases)

every push to `main` ships a new one

every file is the game itself no archives no installers

### windows

| arch | file |
|---|---|
| x64 | `cn_tower_game-windows-x64.exe` |
| x86 | `cn_tower_game-windows-x86.exe` |
| arm64 | `cn_tower_game-windows-arm64.exe` |

### macos

one universal file for apple silicon and intel on macos 11+

`cn_tower_game-macos-universal`

### linux

static so any distro works

`cn_tower_game-linux-<arch>` where arch is one of

| arch | what runs it |
|---|---|
| `x86_64` | your pc |
| `i686` | your old pc |
| `aarch64` | raspberry pi 4 and 5 arm servers baikal-m |
| `armhf` | raspberry pi 2 and 3 on 32 bit |
| `armel` | raspberry pi 1 and zero and old routers |
| `riscv64` | risc-v boards |
| `loongarch64` | loongson |
| `mipsel` | baikal-t1 and routers |
| `mips` | big endian routers |
| `mips64el` `mips64` | 64 bit mips |
| `ppc64le` | ibm power |
| `ppc64` `powerpc` | old power macs and big endian power |
| `s390x` | ibm mainframes |
| `sparc64` | sun and oracle sparc |
| `e2k` | elbrus 2c3 12c and 16c |
| `alpha` `hppa` `m68k` `sh4` | museum pieces |

### bsd and friends

`cn_tower_game-<os>-x86_64` for `freebsd` `openbsd` `netbsd` `dragonflybsd` `solaris` `illumos` `haiku`

### webassembly

`cn_tower_game-wasm32-wasi.wasm` runs anywhere with a wasi runtime

```
wasmtime cn_tower_game-wasm32-wasi.wasm
```

no terminal in wasi so it always plays in plain mode

to keep saves give it the current dir

```
wasmtime --dir=. cn_tower_game-wasm32-wasi.wasm
```

### how its tested

ci plays every build except windows on arm through to the win

linux ones run under qemu and bsd solaris and haiku ones run in real vms

### windows on arm

idgaf about windows on arm

the build exists because it was one line in ci

ci never even runs it so if it breaks thats your problem

### running on unix

browsers drop the executable bit so give it back

```
chmod +x cn_tower_game-linux-x86_64
./cn_tower_game-linux-x86_64
```

the macos build isnt signed so also clear the quarantine flag

```
chmod +x cn_tower_game-macos-universal
xattr -d com.apple.quarantine cn_tower_game-macos-universal
./cn_tower_game-macos-universal
```

## build

windows from a developer command prompt

```
cd src
nmake
```

linux and macos

```
cd src
make
```

bsd solaris haiku or anything else with a c compiler

this builds it with the system `cc` and plays through to the win to prove it works

```
sh tests/ci_build.sh
```

plain c99 and posix (winapi on windows)

if it has a c compiler it builds

### elbrus

started purely for fun and now its a real build

ci builds it with mcst `lcc` 1.31.05 and plays through to the win under `qemu-e2k`

the build targets e2k v6 so its elbrus 2c3 12c and 16c

the toolchain lives in a private prerelease of this repo so forks wont get this build

on the real thing

```
cd src
make CC=lcc
```

## play

type commands like `go north` `buy ticket` `look around`

case doesnt matter

| command | what it does |
|---|---|
| `help` | list commands |
| `look` | describe where you are again |
| `inventory` | money and items |
| `save` / `load` | save and load the game |
| `restart` / `exit` | start over or quit |
| `debug` | money items teleport and sweet+ mode |

one good ending and a bunch of bad ones

run from `src` and saves go to `../data/`

run from anywhere else and they go to the current directory

## interface

in a terminal the game goes full screen

location money and items up top

the story in the middle

hints at the bottom showing only what works right now plus the general commands

| key | action |
|---|---|
| `tab` | complete a command and press again to cycle |
| `→` | accept the grey suggestion |
| `↑` `↓` | command history |
| `ctrl+u` | clear the line |
| `ctrl+d` `ctrl+c` | quit |

`--plain` or `CN_TOWER_PLAIN=1` gives you the classic line by line mode

it also kicks in on its own when output isnt a terminal

## changes from the python version

- no ip based country check and sweet+ mode lives in the debug menu
- dialogue and ascii art are built in
- the edgewalk is actually winnable now
  - just a chill guy is east of the glass floor
  - alexs phone can be returned at the info booth for $100 with `return phone`
  - the lookout has an elevator back down with `go back`
  - alex can be met a second time

## art

`assets/` holds the artwork

`social-preview.png` is this readme header and the repo preview

`cn_tower.ico` gets embedded into the windows exe

the rest are alternatives

the repo preview is set by hand in settings → general → social preview
