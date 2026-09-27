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

| os | arch | file |
|---|---|---|
| windows | x64 | `cn_tower_game-windows-x64.exe` |
| windows | x86 | `cn_tower_game-windows-x86.exe` |
| windows | arm64 | `cn_tower_game-windows-arm64.exe` |
| macos 11+ | apple silicon + intel | `cn_tower_game-macos-universal.tar.gz` |
| linux | x86_64 | `cn_tower_game-linux-x86_64.tar.gz` |
| linux | arm64 / baikal-m | `cn_tower_game-linux-aarch64.tar.gz` |
| linux | armv7 | `cn_tower_game-linux-armhf.tar.gz` |
| linux | x86 | `cn_tower_game-linux-i686.tar.gz` |
| linux | risc-v 64 | `cn_tower_game-linux-riscv64.tar.gz` |
| linux | mips32 le / baikal-t1 | `cn_tower_game-linux-mipsel.tar.gz` |

linux builds are static and run on any distro

ci plays every build except windows on arm through to the win and runs the non x86 linux ones under qemu

### windows on arm

idgaf about windows on arm

the build exists because it was one line in ci

ci never even runs it so if it breaks thats your problem

### macos

the build isnt signed so clear the quarantine flag first

```
xattr -d com.apple.quarantine cn_tower_game
```

## build

windows from a developer command prompt

```
cd src
nmake
```

linux macos bsd

```
cd src
make
```

plain c99 and posix (winapi on windows)

if it has a c compiler it builds

### elbrus

purely for fun

no prebuilt binary and no ci since mcst doesnt hand out `lcc`

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
