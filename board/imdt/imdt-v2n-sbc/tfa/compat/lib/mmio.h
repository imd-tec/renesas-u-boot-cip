/* SPDX-License-Identifier: GPL-2.0+ */
#ifndef TFA_COMPAT_MMIO_H
#define TFA_COMPAT_MMIO_H

#include <stdint.h>
#include <asm/io.h>

#define mmio_read_8(a)		readb((uintptr_t)(a))
#define mmio_read_16(a)		readw((uintptr_t)(a))
#define mmio_read_32(a)		readl((uintptr_t)(a))
#define mmio_read_64(a)		readq((uintptr_t)(a))
#define mmio_write_8(a, v)	writeb((v), (uintptr_t)(a))
#define mmio_write_16(a, v)	writew((v), (uintptr_t)(a))
#define mmio_write_32(a, v)	writel((v), (uintptr_t)(a))
#define mmio_write_64(a, v)	writeq((v), (uintptr_t)(a))

#endif
