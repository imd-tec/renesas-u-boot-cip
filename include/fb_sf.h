/* SPDX-License-Identifier: GPL-2.0+ */
#ifndef _FB_SF_H_
#define _FB_SF_H_

/**
 * fastboot_sf_selected() - Whether flash and erase target SPI NOR
 */
bool fastboot_sf_selected(void);

/**
 * fastboot_sf_select() - Make flash and erase target SPI NOR or the default
 *
 * @nor: true for SPI NOR
 * @response: Pointer to fastboot response buffer
 */
void fastboot_sf_select(bool nor, char *response);

/**
 * fastboot_sf_flash_write() - Write image to SPI NOR for fastboot
 *
 * The partition is named by "fastboot_sf_partition_<name>" holding
 * "<offset> <size>" in bytes.
 *
 * @cmd: Named partition to write image to
 * @download_buffer: Pointer to image data
 * @download_bytes: Size of image data
 * @response: Pointer to fastboot response buffer
 */
void fastboot_sf_flash_write(const char *cmd, void *download_buffer,
			     u32 download_bytes, char *response);

/**
 * fastboot_sf_erase() - Erase a SPI NOR partition for fastboot
 *
 * @cmd: Named partition to erase
 * @response: Pointer to fastboot response buffer
 */
void fastboot_sf_erase(const char *cmd, char *response);
#endif
