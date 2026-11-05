/* SPDX-License-Identifier: GPL-2.0+ */
#ifndef TFA_COMPAT_ARCH_HELPERS_H
#define TFA_COMPAT_ARCH_HELPERS_H

#define dsb()	asm volatile("dsb sy" : : : "memory")
#define isb()	asm volatile("isb" : : : "memory")

#endif
