#!/bin/bash
# Wrap spl/u-boot-spl.bin in the boot ROM SCIF header: v2h-spl-srec.sh <bin> <out.srec>
set -e
bin=$1; out=$2; base=0x08108000
python3 -I - "$bin" "$out.hdr" <<PY
import struct,sys
b=open(sys.argv[1],'rb').read()
h=bytearray(b'\xff'*512)
struct.pack_into('<I',h,0,min(len(b),0x30000)); struct.pack_into('<I',h,0x10,0x200); struct.pack_into('<I',h,0x20,0x08108000)
h[0x1fe]=0x55; h[0x1ff]=0xaa
open(sys.argv[2],'wb').write(h)
PY
srec_cat "$out.hdr" -binary -offset 0x08101E00 "$bin" -binary -offset $base -o - -Motorola -address-length=4 -Output_Block_Size=16 -execution-start-address=$base | grep -v '^S5' > "$out"
