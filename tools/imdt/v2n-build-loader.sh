#!/bin/bash
# Build the V2N SCIF loader image: tools/imdt/v2n-build-loader.sh [out.img]
BOARD=imdt-v2n-sbc exec "$(dirname "$0")/v2h-build-loader.sh" "$@"
