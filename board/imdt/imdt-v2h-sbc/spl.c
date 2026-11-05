// SPDX-License-Identifier: GPL-2.0+
/*
 * RZ/V2H SCIF-loadable loader SPL.
 *
 * The boot ROM runs this from SRAM. It brings up clocks, security and DRAM,
 * taking the DDR PHY firmware over SCIF first, then loads a second stage into
 * DRAM and runs it.
 */

#include <init.h>
#include <spl.h>
#include <asm/io.h>
#include <cpg.h>
#include <ddr.h>

#define CPG_BASE		0x10420000UL
#define CPG_CLKON_8		(CPG_BASE + 0x620)
#define CPG_RST_9		(CPG_BASE + 0x924)
#define CPG_BUS_3_MSTOP		(CPG_BASE + 0xd08)

#define SCIF0			0x11c01400UL
#define SCSMR			0x00
#define SCBRR			0x02
#define SCSCR			0x04
#define SCFTDR			0x06
#define SCFSR			0x08
#define SCFRDR			0x0a
#define SCFCR			0x0c
#define SCLSR			0x12
#define SCSEMR			0x14

/* 100 MHz SCIF clock. */
#define BRR_115200		0x1a, 0xff
#define BRR_921600		0x02, 0xe2

void syc_init(unsigned int freq);
void pfc_setup(void);
void plat_security_setup(void);

/* Upper 16 bits enable the lower 16. */
static void scif0_enable(void)
{
	volatile int i;

	writel(0x80008000, CPG_CLKON_8);
	writel(0x00200020, CPG_RST_9);
	writel(0x40000000, CPG_BUS_3_MSTOP);

	for (i = 0; i < 0x10000; i++)
		;
}

static void scif0_init(unsigned int brr, unsigned int mddr)
{
	writew(0, SCIF0 + SCSCR);
	writew(6, SCIF0 + SCFCR);
	readw(SCIF0 + SCFSR);
	writew(0, SCIF0 + SCFSR);
	readw(SCIF0 + SCLSR);
	writew(0, SCIF0 + SCLSR);
	writew(0, SCIF0 + SCSMR);
	clrbits_8(SCIF0 + SCSEMR, 0x10);
	writeb(brr, SCIF0 + SCBRR);
	setbits_8(SCIF0 + SCSEMR, 0x30);
	writeb(mddr, SCIF0 + SCBRR);
	writew(0, SCIF0 + SCFCR);
	writew(0x30, SCIF0 + SCSCR);
}

static void scif0_putc(char c)
{
	if (c == '\n')
		scif0_putc('\r');

	while (!(readw(SCIF0 + SCFSR) & 0x60))
		;
	writeb(c, SCIF0 + SCFTDR);
	writew(readw(SCIF0 + SCFSR) & ~0x60, SCIF0 + SCFSR);
}

static void scif0_puts(const char *s)
{
	while (*s)
		scif0_putc(*s++);
}

static void scif0_flush(void)
{
	while (!(readw(SCIF0 + SCFSR) & 0x40))
		;
}

static unsigned char scif0_getc(void)
{
	unsigned char c;

	while (!(readw(SCIF0 + SCFSR) & 0x02)) {
		if (readw(SCIF0 + SCFSR) & 0x91)
			writew(readw(SCIF0 + SCFSR) & ~0x91, SCIF0 + SCFSR);
		if (readw(SCIF0 + SCLSR) & 0x01)
			writew(readw(SCIF0 + SCLSR) & ~0x01, SCIF0 + SCLSR);
	}
	c = readb(SCIF0 + SCFRDR);
	writew(readw(SCIF0 + SCFSR) & ~0x02, SCIF0 + SCFSR);

	return c;
}

static unsigned int scif0_get32(void)
{
	unsigned int v = 0, i;

	for (i = 0; i < 4; i++)
		v |= (unsigned int)scif0_getc() << (8 * i);

	return v;
}

static void halt(const char *msg)
{
	scif0_puts(msg);
	for (;;)
		asm volatile("wfi");
}

/* "FBLD", u32 address, u32 length, payload, u32 byte sum. Returns the address. */
static unsigned long load_record(void)
{
	const char *magic = "FBLD";
	unsigned char *dst;
	unsigned int addr, len, sum = 0, i;

	for (i = 0; i < 4;) {
		if (scif0_getc() == magic[i])
			i++;
		else
			i = 0;
	}

	addr = scif0_get32();
	len = scif0_get32();
	dst = (unsigned char *)(unsigned long)addr;
	for (i = 0; i < len; i++) {
		dst[i] = scif0_getc();
		sum += dst[i];
	}

	if (sum != scif0_get32())
		halt("sum FAIL\n");

	return addr;
}

static int dram_check(void)
{
	volatile unsigned int *p = (volatile unsigned int *)0x48000000UL;
	unsigned int i;

	for (i = 0; i < 0x40000; i++)
		p[i] = i * 0x9e3779b1U;
	for (i = 0; i < 0x40000; i++)
		if (p[i] != i * 0x9e3779b1U)
			return -1;

	return 0;
}

extern char __bss_start[], __bss_end[];

void board_init_f(ulong dummy)
{
	char *b;
	unsigned long entry;

	for (b = __bss_start; b < __bss_end; b++)
		*b = 0;

	scif0_enable();
	scif0_init(BRR_115200);

	syc_init(24000000);
	pfc_setup();
	cpg_setup();
	scif0_init(BRR_115200);

	scif0_puts("\nV2H SPL\nbaud\n");
	scif0_flush();
	scif0_init(BRR_921600);

	load_record();
	scif0_puts("phyfw\n");

	plat_security_setup();
	ddr_setup();
	if (dram_check())
		halt("dram FAIL\n");
	scif0_puts("ready\n");

	entry = load_record();
	scif0_puts("go\n");
	scif0_flush();
	((void (*)(unsigned long))entry)(0);

	halt("returned\n");
}

u32 spl_boot_device(void)
{
	return BOOT_DEVICE_UART;
}

void reset_cpu(void)
{
	halt("reset_cpu\n");
}
