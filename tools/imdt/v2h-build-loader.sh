#!/bin/bash
# Build the V2H SCIF loader image: tools/imdt/v2h-build-loader.sh [out.img]
set -e
cd "$(dirname "$0")/../.."
export ARCH=arm CROSS_COMPILE=${CROSS_COMPILE:-aarch64-linux-gnu-}
out=${1:-v2h-fastboot.img}
jobs=-j$(nproc)

make imdt-v2h-sbc-spl_defconfig </dev/null
make $jobs spl/u-boot-spl.bin </dev/null
cp spl/u-boot-spl.bin "$out.spl"

make imdt-v2h-sbc-fastboot_defconfig </dev/null
make $jobs </dev/null
tools/imdt/v2h-mkimg.py "$out" u-boot.bin "$out.spl"
rm -f "$out.spl"
