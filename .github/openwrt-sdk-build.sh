#!/bin/bash
# runs inside the official openwrt sdk container and builds the cn-tower package
# same steps as openwrt/gh-action-sdk except the sdk archive is kept in /cache
# so ci only downloads it once instead of on every run
#
# mounts: /feed is the openwrt dir of the repo /cache keeps the sdk archive
# and /artifacts gets the built packages

set -e
cd /builder

# the release containers ship setup.sh that downloads and unpacks the sdk
if [ -f setup.sh ]; then
    FILE_HOST="${UPSTREAM_URL:-${FILE_HOST:-https://downloads.openwrt.org}}"

    if [ -z "$TARGET" ] || [ -z "$VERSION_PATH" ] || [ -z "$DOWNLOAD_FILE" ]; then
        echo "image doesnt say where its sdk lives so falling back to setup.sh"
        bash setup.sh
    else
        DOWNLOAD_PATH="$VERSION_PATH/targets/$TARGET"

        # the checksum list is tiny so always fetch it fresh and check its signature
        wget -nv "$FILE_HOST/$DOWNLOAD_PATH/sha256sums" -O sha256sums
        wget -nv "$FILE_HOST/$DOWNLOAD_PATH/sha256sums.asc" -O sha256sums.asc
        gpg --import /builder/keys/*.asc
        gpg --with-fingerprint --verify sha256sums.asc sha256sums

        file_name="$(grep "$DOWNLOAD_FILE" sha256sums | cut -d "*" -f 2)"
        if [ -z "$file_name" ]; then
            echo "no file matching $DOWNLOAD_FILE in sha256sums"
            exit 1
        fi
        grep "$file_name" sha256sums > sha256sums_min

        # reuse the cached archive only if its checksum still matches
        if [ -f "/cache/$file_name" ] && (cd /cache && sha256sum -c /builder/sha256sums_min); then
            echo "sdk from cache: $file_name"
        else
            echo "downloading sdk: $file_name"
            rm -f /cache/*
            wget -nv "$FILE_HOST/$DOWNLOAD_PATH/$file_name" -O "/cache/$file_name"
            (cd /cache && sha256sum -c /builder/sha256sums_min)
        fi

        tar xf "/cache/$file_name" --strip=1 --no-same-owner -C .
        rm -rf sha256sums sha256sums_min sha256sums.asc keys setup.sh
    fi
fi

# only our feed so nothing else gets downloaded
echo "src-link cntower /feed" > feeds.conf
./scripts/feeds update -a
make defconfig
./scripts/feeds install -p cntower -f cn-tower
make package/cn-tower/compile -j"$(nproc)"

cp -r bin /artifacts/
