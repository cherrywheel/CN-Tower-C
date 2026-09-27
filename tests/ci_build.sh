#!/bin/sh
# build with whatever c compiler the system has and play through to the win
# used by the ci jobs that run inside bsd solaris and haiku vms
set -e
cd "$(dirname "$0")/../src"

if [ -z "$CC" ]; then
    CC=$(command -v cc || command -v gcc || command -v clang)
fi
echo "compiler: $CC"

$CC -std=c99 -Wall -Wextra -O2 -I../include *.c -o cn_tower_game

CN_TOWER_SEED=1 ./cn_tower_game < ../tests/win_path.txt > ../win_output.txt
if grep "(Win)" ../win_output.txt; then
    exit 0
fi
cat ../win_output.txt
exit 1
