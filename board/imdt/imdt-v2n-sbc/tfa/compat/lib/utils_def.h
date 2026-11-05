/* SPDX-License-Identifier: GPL-2.0+ */
#ifndef TFA_COMPAT_UTILS_DEF_H
#define TFA_COMPAT_UTILS_DEF_H

#include <linux/bitops.h>
#include <linux/kernel.h>

#ifndef U
#define U(x)	x##U
#define UL(x)	x##UL
#define ULL(x)	x##ULL
#endif

#endif
