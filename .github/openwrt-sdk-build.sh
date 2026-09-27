#!/bin/bash
# runs inside the official openwrt sdk container and builds the cn-tower package
# same steps as openwrt/gh-action-sdk except the sdk archive is kept in /cache
# so ci only downloads it once instead of on every run
#
# mounts: /feed is the openwrt dir of the repo /cache keeps the sdk archive
# and /artifacts gets the built packages

set -e

BUILDER_DIR="${BUILDER_DIR:-/builder}"
CACHE_DIR="${CACHE_DIR:-/cache}"
export CACHE_DIR
cd "$BUILDER_DIR"

# wget waits 15 minutes on a stalled mirror by default so give up and retry sooner
printf 'timeout = 60\ntries = 5\nwaitretry = 5\n' > /tmp/wgetrc
export WGETRC=/tmp/wgetrc

# the release containers ship setup.sh that downloads checks and unpacks the sdk
# we keep that script as is and only swap the line that downloads the archive
# so the signature and checksum checks stay exactly the ones openwrt ships
if [ -f setup.sh ]; then
    if grep -q '^wget .*\$file_name' setup.sh; then
        awk '
            /^wget .*\$file_name/ {
                print "if [ -f \"$CACHE_DIR/$file_name\" ] && grep \"$file_name\" sha256sums | (cd \"$CACHE_DIR\" && sha256sum -c -); then"
                print "    echo \"sdk from cache: $file_name\""
                print "    cp \"$CACHE_DIR/$file_name\" ."
                print "else"
                print "    " $0
                print "    rm -f \"$CACHE_DIR\"/*"
                print "    cp \"$file_name\" \"$CACHE_DIR/\""
                print "fi"
                next
            }
            { print }
        ' setup.sh > setup-cached.sh
        bash setup-cached.sh
        rm -f setup-cached.sh
    else
        echo "setup.sh looks different than expected so no cache this time"
        bash setup.sh
    fi
fi

# only our feed so nothing else gets downloaded
echo "src-link cntower /feed" > feeds.conf
./scripts/feeds update -a
make defconfig
./scripts/feeds install -p cntower -f cn-tower
make package/cn-tower/compile -j"$(nproc)"

cp -r bin /artifacts/
