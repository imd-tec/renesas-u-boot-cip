/* SPDX-License-Identifier: GPL-2.0+ */
#ifndef TFA_COMPAT_DEBUG_H
#define TFA_COMPAT_DEBUG_H

void spl_puts(const char *s);
void spl_panic(void) __attribute__((noreturn));

#define ERROR(...)	spl_puts("E\n")
#define WARN(...)	do { } while (0)
#define INFO(...)	do { } while (0)
#define VERBOSE(...)	do { } while (0)
#define NOTICE(...)	do { } while (0)
#define panic()		spl_panic()

#endif
