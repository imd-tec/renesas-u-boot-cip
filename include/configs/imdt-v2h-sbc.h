/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2022 Renesas Electronics Corporation
 */

#ifndef __IMDT_RZV2H_EVK_H
#define __IMDT_RZV2H_EVK_H

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

/* The HF/QSPI layout permits up to 1 MiB large bootloader blob */
#define CONFIG_BOARD_SIZE_LIMIT		1048576

/* ENV setting */
#define CONFIG_SYS_MMC_IMG_LOAD_PART	2

#define MMC_PART 	"mmcpart=" __stringify(CONFIG_SYS_MMC_IMG_LOAD_PART) "\0" 
#define EXTRA_ENV_SETTINGS_BASE	\
	"usb_pgood_delay=2000\0"	\
	"bootm_size=0x10000000\0"	\
	"prodsdbootargs=setenv bootargs rw rootwait earlycon root=/dev/mmcblk1p${mmcpart} \0" \
	"prodemmcbootargs=setenv bootargs rw rootwait earlycon root=/dev/mmcblk0p${mmcpart} \0" \
	"bootimage=run set_pmic; booti 0x48080000 - 0x48000000 \0" \
	"set_pmic=i2c dev 8; i2c mw 0x6a 0x22 0x0f; i2c mw 0x6a 0x24 0x00; i2c md 0x6a 0x00 0x30; i2c mw 0x12 0x8D 0x02; i2c md 0x12 0x20 0x80 \0" \
	"bootcmd_check=if mmc dev 1; then run sd1load; else run emmcload; fi \0"

#if defined(CONFIG_RZ_CODECS_IMDT_V2H) 
#define CODECS_FEATURE \
	"codaddr=0xC7D00000 \0"     \
	"codbin=Codec_Bin.bin \0"
#define EMMC_LOAD_COMMAND_CODEC "ext4load mmc 0:${mmcpart} ${codaddr} boot/${codbin}; "
#define SD1_LOAD_COMMAND_CODEC "ext4load mmc 1:${mmcpart} ${codaddr} boot/${codbin}; "
#else
	#define CODECS_FEATURE ""
	#define EMMC_LOAD_COMMAND_CODEC ""
	#define SD1_LOAD_COMMAND_CODEC ""
#endif

#if defined(CONFIG_RZ_OPENCVA_IMDT_V2H) 
#define OCA_FEATURE \
 	"ocaaddr=0xC0000000 \0"     \
 	"ocabin=OpenCV_Bin.bin \0"  
 	#define EMMC_LOAD_COMMAND_OPENCVA "ext4load mmc 0:${mmcpart} ${ocaaddr} boot/${ocabin}; "
 	#define SD1_LOAD_COMMAND_OPENCVA "ext4load mmc 1:${mmcpart} ${ocaaddr} boot/${ocabin};"
#else
	#define OCA_FEATURE "" 
	#define EMMC_LOAD_COMMAND_OPENCVA ""
	#define SD1_LOAD_COMMAND_OPENCVA ""
#endif /* CONFIG_RZ_OPENCVA_IMDT_V2H */ 

#if defined(CONFIG_RZ_DRPAI_IMDT_V2H)
#define DRPAI_FEATURE \
	"ipaddr=192.168.1.11\0" \
	"serverip=192.168.1.10\0" \
	"netmask=255.255.255.0\0" \
	"ethaddr=02:11:22:33:44:55\0" \
	"eth1addr=02:11:22:33:44:66\0"
#else 
	#define DRPAI_FEATURE ""
#endif /* CONFIG_RZ_DRPAI_IMDT_V2H */ \

#define EMMC_LOAD_COMMANDS "emmcload=" \
	EMMC_LOAD_COMMAND_CODEC \
	EMMC_LOAD_COMMAND_OPENCVA \
	"ext4load mmc 0:${mmcpart} 0x48080000 boot/Image;ext4load mmc 0:${mmcpart} 0x48000000 boot/imdt-v2h-sbc.dtb;run prodemmcbootargs \0" 
#define SD1_LOAD_COMMAND \
	"sd1load=" \
	SD1_LOAD_COMMAND_CODEC \
	SD1_LOAD_COMMAND_OPENCVA \
	"ext4load mmc 1:${mmcpart} 0x48080000 boot/Image;ext4load mmc 1:${mmcpart} 0x48000000 boot/imdt-v2h-sbc.dtb;run prodsdbootargs \0" 

#define CONFIG_EXTRA_ENV_SETTINGS \
	MMC_PART \
	DRPAI_FEATURE \
	CODECS_FEATURE \
	OCA_FEATURE \
 	EXTRA_ENV_SETTINGS_BASE \
	EMMC_LOAD_COMMANDS \
	SD1_LOAD_COMMAND 

#define CONFIG_BOOTCOMMAND	"run bootcmd_check; run bootimage"

/* For board */
/* Ethernet RAVB */
#define CONFIG_BITBANGMII_MULTI

#endif /* __IMDT_RZV2H_EVK_H */
