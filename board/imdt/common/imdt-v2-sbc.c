#include <common.h>
#include <command.h>
#include <cpu_func.h>
#include <env.h>
#include <image.h>
#include <init.h>
#include <malloc.h>
#include <netdev.h>
#include <dm.h>
#include <dm/platform_data/serial_sh.h>
#include <asm/processor.h>
#include <asm/mach-types.h>
#include <asm/io.h>
#include <linux/bitops.h>
#include <linux/errno.h>
#include <asm/arch/sys_proto.h>
#include <asm/gpio.h>
#include <asm/arch/gpio.h>
#include <asm/arch/renesas.h>
#include <asm/arch/rcar-mstp.h>
#include <asm/arch/sh_sdhi.h>
#include <i2c.h>
#include <mmc.h>
#include <linux/delay.h>
#include <renesas/rzf-dev/mmio.h>
#define SYS_BASE			0x10430000
#define	SYS_LSI_MODE						(SYS_BASE + 0x00000300)
uint8_t sys_get_boot_mode(void);
// Override of weak function to dynamiclaly get device when storing env in FAT.
// in order for this to be called, the firstcharacter assigned to 
// CONFIG_ENV_FAT_DEVICE_AND_PART=":1" 
// must be a ':'
int mmc_get_env_dev(void)
{
	return (int)sys_get_boot_mode();
}

int set_boot_dev_env_var(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
	char boot_mode_str[32];
    // Example: Just print the boot mode
    uint8_t mode = sys_get_boot_mode();

	// Make sure the string is long enough
    snprintf(boot_mode_str, sizeof(boot_mode_str), "%d", mode);
    env_set("boot_device", boot_mode_str);  // Set the environment variable
    // Print out the boot mode value
    printf("Current boot mode: %d\n", mode);

    return 0;
}

uint8_t sys_get_boot_mode(void)
{
	uint8_t device_number = 0xFFU;
	uint8_t boot_dev = mmio_read_32(SYS_LSI_MODE) & SYS_LSI_MODE_MASK;
	
	switch (boot_dev) {
	case (SYS_LSI_MODE_EMMC18):
	case (SYS_LSI_MODE_EMMC33):
		device_number = 0;
		printf("u-boot loaded from eMMC\n");
		break;
	case (SYS_LSI_MODE_SFLASH18):
	case (SYS_LSI_MODE_SFLASH33):
		device_number = 1;
		printf("u-boot loaded from QSPI\n");
		break;
	}
	return device_number;
}

U_BOOT_CMD(
    setbootdevice,        // Command name
    1,               // Max arguments
    0,               // Command flags (0 means no special flags)
    set_boot_dev_env_var,    // Function to call
    "Display boot mode", // Short description
    "  - Displays the current boot mode."  // Help text
);

