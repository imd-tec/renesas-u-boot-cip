#
# board/imdt/imdt-v2n-sbc/Makefile
#
# Copyright (C) 2022 Renesas Electronics Corporation
#
# SPDX-License-Identifier: GPL-2.0+
#

ifdef CONFIG_SPL_BUILD
obj-y	:= ../imdt-v2h-sbc/spl.o ../imdt-v2h-sbc/tfa/glue.o
obj-y	+= tfa/cpg.o tfa/pfc.o tfa/plat_security.o tfa/tzc400.o
obj-y	+= tfa/ddr/ddr.o tfa/ddr/ddr_misc.o tfa/ddr/ddr_setup_lpddr4.o
obj-y	+= tfa/ddr/ddr_param_def_lpddr4.o tfa/ddr/ddr_retcsr_lpddr4.o
ccflags-y += -I$(srctree)/$(src)/tfa/compat -I$(srctree)/$(src)/tfa/inc \
	     -I$(srctree)/$(src)/tfa/ddr
else
obj-y	:= imdt-v2n-sbc.o ../common/imdt-v2-sbc.o ../../renesas/rcar-common/common.o
endif
