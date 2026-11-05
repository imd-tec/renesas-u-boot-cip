// SPDX-License-Identifier: GPL-2.0+
/*
 * Fastboot flash provider for SPI NOR, chosen at run time with "oem storage".
 */

#include <common.h>
#include <dm.h>
#include <env.h>
#include <fastboot.h>
#include <fb_sf.h>
#include <spi.h>
#include <spi_flash.h>
#include <linux/kernel.h>
#include <linux/string.h>

static struct spi_flash *flash;
static bool selected;

bool fastboot_sf_selected(void)
{
	return selected;
}

static struct spi_flash *sf_probe(char *response)
{
	struct udevice *dev;

	if (flash)
		return flash;

	if (spi_flash_probe_bus_cs(CONFIG_SF_DEFAULT_BUS,
				   CONFIG_SF_DEFAULT_CS, &dev)) {
		fastboot_fail("Cannot probe SPI NOR", response);
		return NULL;
	}

	flash = dev_get_uclass_priv(dev);
	return flash;
}

void fastboot_sf_select(bool nor, char *response)
{
	if (nor && !sf_probe(response))
		return;

	selected = nor;
	fastboot_okay(NULL, response);
}

static struct spi_flash *sf_part(const char *name, ulong *off, ulong *size,
				 char *response)
{
	char var[64];
	const char *val;
	char *end;

	if (!name) {
		fastboot_fail("Expected partition name", response);
		return NULL;
	}

	snprintf(var, sizeof(var), "fastboot_sf_partition_%s", name);
	val = env_get(var);
	if (!val) {
		fastboot_fail("Unknown SPI NOR partition", response);
		return NULL;
	}

	*off = simple_strtoul(val, &end, 0);
	*size = simple_strtoul(skip_spaces(end), NULL, 0);
	if (!*size) {
		fastboot_fail("Bad SPI NOR partition", response);
		return NULL;
	}

	if (!sf_probe(response))
		return NULL;

	if (*off % flash->erase_size || *size % flash->erase_size) {
		fastboot_fail("Partition not erase aligned", response);
		return NULL;
	}

	return flash;
}

void fastboot_sf_flash_write(const char *cmd, void *download_buffer,
			     u32 download_bytes, char *response)
{
	ulong off, size;

	if (!sf_part(cmd, &off, &size, response))
		return;

	if (download_bytes > size) {
		fastboot_fail("Image too large for partition", response);
		return;
	}

	printf("Writing %u bytes to SPI NOR %s at 0x%lx\n", download_bytes,
	       cmd, off);
	if (spi_flash_erase(flash, off, ALIGN(download_bytes, flash->erase_size)) ||
	    spi_flash_write(flash, off, download_bytes, download_buffer)) {
		fastboot_fail("Cannot write SPI NOR", response);
		return;
	}

	fastboot_okay(NULL, response);
}

void fastboot_sf_erase(const char *cmd, char *response)
{
	ulong off, size;

	if (!sf_part(cmd, &off, &size, response))
		return;

	printf("Erasing SPI NOR %s at 0x%lx\n", cmd, off);
	if (spi_flash_erase(flash, off, size))
		fastboot_fail("Cannot erase SPI NOR", response);
	else
		fastboot_okay(NULL, response);
}
