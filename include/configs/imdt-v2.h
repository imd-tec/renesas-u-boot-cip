/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2022 Renesas Electronics Corporation
   Copyright (C) 2025 IMDT Ltd

    Common header for the IMDT V2H and V2N Single board computers
 */
#ifndef IMDT_COMMON
#define IMDT_COMMON
#include <asm/arch/rmobile.h>

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
#define CONFIG_SYS_MAXARGS		64
#define CONFIG_SYS_BAUDRATE_TABLE	{ 115200, 38400 }

/* PHY needs a longer autoneg timeout */
#define PHY_ANEG_TIMEOUT		20000

/* MEMORY */
#define CONFIG_SYS_INIT_SP_ADDR		CONFIG_SYS_TEXT_BASE

/* SDHI clock freq */
#define CONFIG_SH_SDHI_FREQ		133000000

#define DRAM_RSV_SIZE			0x08000000
#define CONFIG_SYS_SDRAM_BASE		(0x40000000 + DRAM_RSV_SIZE)
#define CONFIG_SYS_SDRAM_SIZE		(0x200000000u - DRAM_RSV_SIZE) //total 8GB
#define CONFIG_SYS_LOAD_ADDR		0x58000000
#define CONFIG_LOADADDR			CONFIG_SYS_LOAD_ADDR // Default load address for tftp, bootp...
#define CONFIG_VERY_BIG_RAM
#define CONFIG_MAX_MEM_MAPPED		(0x80000000u - DRAM_RSV_SIZE)

#define CONFIG_SYS_MONITOR_BASE		0x00000000
#define CONFIG_SYS_MONITOR_LEN		(1 * 1024 * 1024)
#define CONFIG_SYS_MALLOC_LEN		(64 * 1024 * 1024)
#define CONFIG_SYS_BOOTM_LEN		(64 << 20)

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

/* ENV setting */
#define EXTRA_ENV_SETTINGS_BASE	\
	"mmcpart=2\0"				\
	"overwrite_bl=0\0"			\
	"usb_pgood_delay=2000\0"	\
	"bootm_size=0x10000000\0"	\
	"prodbootargs=setenv bootargs rw rootwait earlycon root=/dev/mmcblk${boot_device}p${mmcpart} \0" \
	"bootimage=run set_pmic; booti 0x48080000 - 0x48000000 \0" \
	"overlay_list="__stringify(CONFIG_ADD_OVERLAYS)"\0" \
	"set_pmic=i2c dev 8; i2c mw 0x6a 0x22 0x0f; i2c mw 0x6a 0x24 0x00; i2c md 0x6a 0x00 0x30; i2c mw 0x12 0x8D 0x02; i2c md 0x12 0x20 0x80 \0" \
	"overlay_addr=0x48070000 \0" \
	"apply_overlays=" \
    "fdt resize 0x60000; " \
    "for overlay in ${overlay_list}; do " \
        "echo Applying overlay ${overlay}; " \
        "ext4load mmc ${boot_device}:${mmcpart} ${overlay_addr} boot/${overlay}; " \
        "fdt apply ${overlay_addr}; " \
    "done\0" \

#define CODECS_FEATURE ""
#define LOAD_COMMAND_CODEC ""

#define OCA_FEATURE "" 
#define LOAD_COMMAND_OPENCVA ""

#if defined(CONFIG_RZ_FEATURES_DRPAI)
#define DRPAI_FEATURE \
	"ipaddr=192.168.1.11\0" \
	"serverip=192.168.1.10\0" \
	"netmask=255.255.255.0\0" \
	"ethaddr=02:11:22:33:44:55\0" \
	"eth1addr=02:11:22:33:44:66\0"
#else 
	#define DRPAI_FEATURE ""
#endif /* CONFIG_RZ_FEATURES_DRPAI */

#define BOOT_COMMAND \
	"boot_command="	\
	LOAD_COMMAND_CODEC \
	LOAD_COMMAND_OPENCVA \
	"ext4load mmc ${boot_device}:${mmcpart} 0x48080000 boot/Image; ext4load mmc ${boot_device}:${mmcpart} 0x48000000 boot/" \
	CONFIG_DEFAULT_FDT_FILE \
	"; fdt addr 0x48000000;" \
	"run apply_overlays; \0" \
	""

#define CONFIG_EXTRA_ENV_SETTINGS \
	DRPAI_FEATURE \
	CODECS_FEATURE \
	OCA_FEATURE \
 	EXTRA_ENV_SETTINGS_BASE \
	BOOT_COMMAND \

#define CONFIG_BOOTCOMMAND	"setbootdevice; run boot_command; run prodbootargs; run bootimage"

/* For board */
/* Ethernet RAVB */
#define CONFIG_BITBANGMII_MULTI

#endif /* __IMDT_RZV2H_EVK_H */
