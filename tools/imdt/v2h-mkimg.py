#!/usr/bin/env python3
"""Bundle the V2H SCIF loader into one image: v2h-mkimg.py out.img [stage2.bin [spl.bin]]

Layout: "V2HF", u32 srec_len, u32 phyfw_len, u32 stage2_len, u32 stage2_addr,
then the SPL srec, the DDR PHY firmware and the second stage.
"""
import struct
import subprocess
import sys
import tempfile
import os

root = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")
phyfw = os.path.join(root, "board", "imdt", "imdt-v2h-sbc", "tfa", "ddr", "phyinit_lpddr4.bin")
mk = os.path.join(root, "tools", "imdt", "v2h-spl-srec.sh")
out = sys.argv[1]
stage2 = sys.argv[2] if len(sys.argv) > 2 else os.path.join(root, "u-boot.bin")
spl = sys.argv[3] if len(sys.argv) > 3 else os.path.join(root, "spl", "u-boot-spl.bin")

with tempfile.TemporaryDirectory() as d:
    srec = os.path.join(d, "spl.srec")
    subprocess.run([mk, spl, srec], check=True)
    srec_data = open(srec, "rb").read()

fw = open(phyfw, "rb").read()
s2 = open(stage2, "rb").read()
with open(out, "wb") as f:
    f.write(b"V2HF" + struct.pack("<IIII", len(srec_data), len(fw), len(s2), 0x50000000))
    f.write(srec_data + fw + s2)
print("%s: spl srec %d, phyfw %d, stage2 %d bytes" % (out, len(srec_data), len(fw), len(s2)))
