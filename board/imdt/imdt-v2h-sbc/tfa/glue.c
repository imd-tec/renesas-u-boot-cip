// SPDX-License-Identifier: GPL-2.0+
/*
 * Glue between the TF-A RZ/V2H drivers and the SPL.
 */

#include <stdint.h>
#include <stdbool.h>
#include <asm/io.h>
#include <lib/mmio.h>
#include <sys.h>
#include <sys_regs.h>
#include <rz_soc_def.h>
#include <drivers/delay_timer.h>
#include <common/debug.h>

#define SYC_CNTCR	0x000
#define SYC_CNTFID0	0x020

#define SCIF0		0x11c01400UL

static inline uint64_t cntpct(void)
{
	uint64_t v;

	asm volatile("isb; mrs %0, cntpct_el0" : "=r" (v));
	return v;
}

void syc_init(unsigned int freq)
{
	mmio_write_32(RZ_SOC_SYC_BASE + SYC_CNTFID0, freq);
	mmio_write_32(RZ_SOC_SYC_BASE + SYC_CNTCR, 1);
	asm volatile("msr cntfrq_el0, %0" : : "r" ((uint64_t)freq));
}

void tfa_udelay(unsigned long usec)
{
	uint64_t end = cntpct() + (uint64_t)usec * (RZV2H_SYC_INCK_HZ / 1000000);

	while (cntpct() < end)
		;
}

void tfa_mdelay(unsigned long msec)
{
	while (msec--)
		tfa_udelay(1000);
}

bool sys_is_resume_reboot(void)
{
	return false;
}

boot_mode_t sys_get_boot_mode(void)
{
	switch (mmio_read_32(SYS_LSI_MODE) & SYS_LSI_MODE_MASK) {
	case SYS_LSI_MODE_ESD:
	case SYS_LSI_MODE_ESD_2:
		return SYS_BOOT_MODE_ESD;
	case SYS_LSI_MODE_EMMC18:
		return SYS_BOOT_MODE_EMMC_1_8;
	case SYS_LSI_MODE_EMMC33:
		return SYS_BOOT_MODE_EMMC_3_3;
	case SYS_LSI_MODE_SFLASH18:
		return SYS_BOOT_MODE_SPI_1_8;
	case SYS_LSI_MODE_SFLASH33:
		return SYS_BOOT_MODE_SPI_3_3;
	default:
		return SYS_BOOT_MODE_SCIF;
	}
}

static void scif0_putc(char c)
{
	while (!(readw(SCIF0 + 0x08) & 0x60))
		;
	writeb(c, SCIF0 + 0x06);
	writew(readw(SCIF0 + 0x08) & ~0x60, SCIF0 + 0x08);
}

void spl_puts(const char *s)
{
	while (*s) {
		if (*s == '\n')
			scif0_putc('\r');
		scif0_putc(*s++);
	}
}

void spl_panic(void)
{
	spl_puts("panic\n");
	for (;;)
		asm volatile("wfi");
}

#include <ddr.h>

uint32_t ddr_csr_table[RET_CSR_SIZE] __attribute__((aligned(8)));
