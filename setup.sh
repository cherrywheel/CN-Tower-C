#!/bin/sh
# sets up everything to build and play cn tower on any unix like system
# linux macos bsd solaris illumos haiku termux wsl msys2 and friends
# on windows use setup.cmd instead
#
# usage: sh setup.sh [options]
#   --yes        install whatever is missing without asking
#   --no-test    skip playing through to the win after the build
#   --cross      debian and ubuntu only: install every cross compiler and qemu
#                that ci uses and play through to the win on each linux arch
#                loongarch64 joins in when zig is around (pip install ziglang)
#   --out DIR    with --cross keep every static binary in DIR
#                as cn_tower_game-linux-<arch>
#   --help       show this

set -e

YES=0
TEST=1
CROSS=0
OUT=""

while [ $# -gt 0 ]; do
    arg=$1
    shift
    case "$arg" in
        --yes|-y) YES=1 ;;
        --no-test) TEST=0 ;;
        --cross) CROSS=1 ;;
        --out)
            if [ $# -eq 0 ]; then
                echo "--out needs a directory"
                exit 1
            fi
            OUT=$1
            shift
            ;;
        --help|-h)
            awk 'NR > 1 && /^#/ { sub(/^# ?/, ""); print; next } NR > 1 { exit }' "$0"
            exit 0
            ;;
        *)
            echo "unknown option $arg (try --help)"
            exit 1
            ;;
    esac
done

if [ -n "$OUT" ]; then
    mkdir -p "$OUT"
    OUT=$(cd "$OUT" && pwd)
fi

cd "$(dirname "$0")"
ROOT=$(pwd)

say() {
    printf '%s\n' "$*"
}

step() {
    printf '\n== %s\n' "$*"
}

have() {
    command -v "$1" >/dev/null 2>&1
}

# ask before doing something with root or packages
confirm() {
    if [ "$YES" = 1 ]; then
        return 0
    fi
    if [ ! -t 0 ]; then
        say "not a terminal so not touching anything (rerun with --yes)"
        return 1
    fi
    printf '%s [y/N] ' "$*"
    read -r answer || return 1
    case "$answer" in
        y|Y|yes|YES) return 0 ;;
        *) return 1 ;;
    esac
}

# run as root through sudo or doas when needed
as_root() {
    if [ "$(id -u 2>/dev/null || echo 0)" = 0 ]; then
        "$@"
    elif have sudo; then
        sudo "$@"
    elif have doas; then
        doas "$@"
    else
        say "need root for: $*"
        return 1
    fi
}

# --- what are we running on ---

step "looking around"

OS=$(uname -s 2>/dev/null || echo unknown)
ARCH=$(uname -m 2>/dev/null || echo unknown)
DISTRO=""
if [ -r /etc/os-release ]; then
    DISTRO=$(. /etc/os-release && echo "${PRETTY_NAME:-$NAME}")
fi
if [ -n "${TERMUX_VERSION:-}" ] || [ -d /data/data/com.termux ]; then
    DISTRO="termux"
fi
say "system: $OS $ARCH ${DISTRO:+($DISTRO)}"

if [ -r /etc/openwrt_release ]; then
    say "this is openwrt and routers dont carry a compiler"
    say "grab the package for your router from releases instead"
    say "  grep ARCH /etc/openwrt_release"
    say "  opkg install cn-tower-openwrt-24.10-<arch>.ipk"
    say "  apk add --allow-untrusted cn-tower-openwrt-25.12-<arch>.apk"
    exit 0
fi

# --- compiler ---

find_cc() {
    if [ -n "${CC:-}" ] && have "${CC%% *}"; then
        echo "$CC"
        return
    fi
    for c in cc gcc clang lcc; do
        if have "$c"; then
            echo "$c"
            return
        fi
    done
}

# prints the command that installs a c compiler and make on this system
install_cmd() {
    if [ "$DISTRO" = termux ]; then echo "pkg install -y clang make"; return; fi
    case "$OS" in
        Darwin) echo "xcode-select --install"; return ;;
        FreeBSD|DragonFly) echo "pkg install -y gmake"; return ;;
        OpenBSD) echo "pkg_add gmake"; return ;;
        NetBSD) echo "pkgin -y install gmake"; return ;;
        SunOS) echo "pkg install developer/gcc"; return ;;
        Haiku) echo "pkgman install -y gcc make"; return ;;
    esac
    if have apt-get; then echo "apt-get install -y build-essential"
    elif have dnf; then echo "dnf install -y gcc make"
    elif have yum; then echo "yum install -y gcc make"
    elif have pacman; then echo "pacman -S --needed --noconfirm base-devel"
    elif have zypper; then echo "zypper install -y gcc make"
    elif have apk; then echo "apk add build-base"
    elif have xbps-install; then echo "xbps-install -y base-devel"
    elif have emerge; then echo "emerge --noreplace sys-devel/gcc dev-build/make"
    elif have eopkg; then echo "eopkg install -y -c system.devel"
    elif have swupd; then echo "swupd bundle-add c-basic"
    elif have nix-env; then echo "nix-env -iA nixpkgs.gcc nixpkgs.gnumake"
    elif have brew; then echo "brew install gcc make"
    fi
}

step "c compiler"

CC_FOUND=$(find_cc)
if [ -z "$CC_FOUND" ]; then
    INSTALL=$(install_cmd)
    if [ -z "$INSTALL" ]; then
        say "no c compiler and no package manager i know"
        say "install any c99 compiler and rerun this"
        exit 1
    fi
    say "no c compiler yet"
    if confirm "install one with: $INSTALL"; then
        case "$INSTALL" in
            xcode-select*|"pkg install -y clang make") $INSTALL ;;
            *) as_root $INSTALL ;;
        esac
    else
        say "ok then run it yourself: $INSTALL"
        exit 1
    fi
    CC_FOUND=$(find_cc)
    if [ -z "$CC_FOUND" ]; then
        say "still no compiler (on macos finish the xcode install and rerun)"
        exit 1
    fi
fi
say "using $CC_FOUND"

# --- build ---

step "building"

GMAKE=""
for m in gmake make; do
    if have "$m" && "$m" --version 2>/dev/null | grep GNU >/dev/null; then
        GMAKE=$m
        break
    fi
done

build_native() {
    if [ -n "$GMAKE" ]; then
        $GMAKE CC="$CC_FOUND"
    else
        # no gnu make so compile it by hand which is all the makefile does anyway
        $CC_FOUND -std=c99 -Wall -Wextra -O2 -I../include *.c -o cn_tower_game
    fi
}

cd "$ROOT/src"
if [ -z "$GMAKE" ]; then
    say "no gnu make so calling $CC_FOUND directly"
fi
build_native

# --- test ---

play_to_win() {
    # $1 is an optional runner like qemu-mips
    out="$ROOT/win_output.txt"
    CN_TOWER_SEED=1 $1 ./cn_tower_game < "$ROOT/tests/win_path.txt" > "$out" 2>&1 || true
    if grep "(Win)" "$out" >/dev/null; then
        rm -f "$out"
        return 0
    fi
    cat "$out"
    return 1
}

if [ "$TEST" = 1 ]; then
    step "playing through to the win"
    if play_to_win ""; then
        say "won the game so the build works"
    else
        say "the build runs but didnt win something is off"
        exit 1
    fi
fi

# --- cross builds like ci ---

if [ "$CROSS" = 1 ]; then
    step "cross builds"
    if ! have apt-get; then
        say "--cross needs debian or ubuntu since it installs their cross compilers"
        exit 1
    fi

    # arch compiler package qemu where - means none needed
    # zig:<target> builds with zig from pypi when its installed
    TARGETS="
x86_64 gcc - -
i686 i686-linux-gnu-gcc libc6-dev-i386-cross qemu-i386
aarch64 aarch64-linux-gnu-gcc libc6-dev-arm64-cross qemu-aarch64
armhf arm-linux-gnueabihf-gcc libc6-dev-armhf-cross qemu-arm
armel arm-linux-gnueabi-gcc libc6-dev-armel-cross qemu-arm
riscv64 riscv64-linux-gnu-gcc libc6-dev-riscv64-cross qemu-riscv64
loongarch64 zig:loongarch64-linux-musl - qemu-loongarch64
mipsel mipsel-linux-gnu-gcc libc6-dev-mipsel-cross qemu-mipsel
mips mips-linux-gnu-gcc libc6-dev-mips-cross qemu-mips
mips64el mips64el-linux-gnuabi64-gcc libc6-dev-mips64el-cross qemu-mips64el
mips64 mips64-linux-gnuabi64-gcc libc6-dev-mips64-cross qemu-mips64
ppc64le powerpc64le-linux-gnu-gcc libc6-dev-ppc64el-cross qemu-ppc64le
ppc64 powerpc64-linux-gnu-gcc libc6-dev-ppc64-cross qemu-ppc64
powerpc powerpc-linux-gnu-gcc libc6-dev-powerpc-cross qemu-ppc
s390x s390x-linux-gnu-gcc libc6-dev-s390x-cross qemu-s390x
sparc64 sparc64-linux-gnu-gcc libc6-dev-sparc64-cross qemu-sparc64
alpha alpha-linux-gnu-gcc libc6-dev-alpha-cross qemu-alpha
hppa hppa-linux-gnu-gcc libc6-dev-hppa-cross qemu-hppa
m68k m68k-linux-gnu-gcc libc6-dev-m68k-cross qemu-m68k
sh4 sh4-linux-gnu-gcc libc6-dev-sh4-cross qemu-sh4
"
    PKGS="qemu-user"
    for pkg in $(echo "$TARGETS" | awk 'NF && $3 != "-" { print "gcc-" substr($2, 1, length($2) - 4), $3 }'); do
        PKGS="$PKGS $pkg"
    done

    if confirm "install cross compilers and qemu with apt"; then
        as_root apt-get update -qq
        as_root apt-get install -y --no-install-recommends $PKGS
    else
        say "skipping cross builds"
        exit 0
    fi

    HAVE_ZIG=0
    if have python3 && python3 -m ziglang version >/dev/null 2>&1; then
        HAVE_ZIG=1
    fi

    CROSS_CFLAGS=${CFLAGS:--std=c99 -O2}
    rm -f "$ROOT/cross_failed.txt"
    echo "$TARGETS" | while read -r arch cc pkg qemu; do
        [ -n "$arch" ] || continue
        case "$cc" in
            zig:*)
                if [ "$HAVE_ZIG" = 0 ]; then
                    say "$arch skipped since theres no zig (pip install ziglang)"
                    continue
                fi
                cc="python3 -m ziglang cc -target ${cc#zig:}"
                ;;
        esac
        [ "$qemu" = "-" ] && qemu=""
        make -s clean
        if make -s CC="$cc" CFLAGS="$CROSS_CFLAGS" LDFLAGS="-static" && play_to_win "$qemu"; then
            say "$arch ok"
            if [ -n "$OUT" ]; then
                cp cn_tower_game "$OUT/cn_tower_game-linux-$arch"
            fi
        else
            say "$arch FAILED"
            echo "$arch" >> "$ROOT/cross_failed.txt"
        fi
    done
    make -s clean
    build_native >/dev/null
    if [ -f "$ROOT/cross_failed.txt" ]; then
        FAILED=$(tr '\n' ' ' < "$ROOT/cross_failed.txt")
        rm -f "$ROOT/cross_failed.txt"
        say "failed: $FAILED"
        exit 1
    fi
    say "every linux arch won the game"
fi

# --- done ---

step "all set"
say "play it"
say "  cd src && ./cn_tower_game"
