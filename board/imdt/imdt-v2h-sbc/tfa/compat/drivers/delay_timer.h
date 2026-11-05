/* SPDX-License-Identifier: GPL-2.0+ */
#ifndef TFA_COMPAT_DELAY_TIMER_H
#define TFA_COMPAT_DELAY_TIMER_H

void tfa_udelay(unsigned long usec);
void tfa_mdelay(unsigned long msec);

#undef udelay
#undef mdelay
#define udelay(x)	tfa_udelay(x)
#define mdelay(x)	tfa_mdelay(x)

#endif
