/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2022 Renesas Electronics Corporation
   Copyright (C) 2026 IMDT Ltd

    Common header for the IMDT V2H and V2N Single board computers
 */
#ifndef IMDT_COMMON
#define IMDT_COMMON
#include <asm/arch/renesas.h>

#define CONFIG_REMAKE_ELF

#ifdef CONFIG_SPL
#define CONFIG_SPL_TARGET	"spl/u-boot-spl.scif"
#endif

/* boot option */
#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG

/* Generic Interrupt Controller Definitions */
/* RZ/V2H use GIC-v3 */
#define CONFIG_GICV3
#define GICD_BASE	0x14900000
#define GICR_BASE	0x14940000

/* console */
#define CONFIG_SYS_CBSIZE		2048
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE
#define CONFIG_SYS_BAUDRATE_TABLE	{ 115200, 38400 }

/* PHY needs a longer autoneg timeout */
#define PHY_ANEG_TIMEOUT		20000

/* MEMORY */
#define CONFIG_SYS_INIT_SP_ADDR		CONFIG_SYS_TEXT_BASE

#define	DRAM_RSV_SIZE			0x08000000
#define	CFG_MAX_MEM_MAPPED		(0x80000000u - DRAM_RSV_SIZE)

/* SDHI clock freq */
#define CONFIG_SH_SDHI_FREQ		133000000

#define SYS_LSI_MODE_ESD					(0)
#define SYS_LSI_MODE_EMMC33					(1)
#define SYS_LSI_MODE_SFLASH33				(2)
#define SYS_LSI_MODE_SCIF					(3)
#define SYS_LSI_MODE_ESD_2					(4)		/* Same as SYS_LSI_MODE_ESD */
#define SYS_LSI_MODE_EMMC18					(5)
#define SYS_LSI_MODE_SFLASH18				(6)
#define SYS_LSI_MODE_SCIF_2					(7)		/* Same as SYS_LSI_MODE_SCIF */
#define SYS_LSI_MODE_COUNT					(8)
#define SYS_LSI_MODE_MASK					(0x7)


/* The HF/QSPI layout permits up to 1 MiB large bootloader blob */
#define CONFIG_BOARD_SIZE_LIMIT		1048576

/*
 * The default environment lives in board/imdt/<board>/<board>.env, which pulls
 * in the shared include/env/imdt/v2-sbc.env. Nothing is defined here.
 */

/* For board */
/* Ethernet RAVB */
#define CONFIG_BITBANGMII_MULTI

#endif /* __IMDT_RZV2H_EVK_H */
