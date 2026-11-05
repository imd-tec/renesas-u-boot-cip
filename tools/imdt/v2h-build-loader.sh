#!/bin/bash
# Build the SCIF loader image: [BOARD=imdt-v2n-sbc] tools/imdt/v2h-build-loader.sh [out.img]
set -e
cd "$(dirname "$0")/../.."
export ARCH=arm CROSS_COMPILE=${CROSS_COMPILE:-aarch64-linux-gnu-}
board=${BOARD:-imdt-v2h-sbc}
out=${1:-${board#imdt-}-fastboot.img}
jobs=-j$(nproc)

make $board-spl_defconfig </dev/null
make $jobs spl/u-boot-spl.bin </dev/null
cp spl/u-boot-spl.bin "$out.spl"

make $board-fastboot_defconfig </dev/null
make $jobs </dev/null
tools/imdt/v2h-mkimg.py "$out" u-boot.bin "$out.spl"
rm -f "$out.spl"
