/*
 * Renesas USB driver R-Car Gen. 3 initialization and power control
 *
 * Copyright (C) 2016 Renesas Electronics Corporation
 *
 * Ported to u-boot
 * Copyright (C) 2016 GlobalLogic
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <asm/io.h>
#include "common.h"
#include "rcar3.h"

#define LPSTS		0x102
#define UGCTRL2		0x184	/* 32-bit register */

/* Low Power Status register (LPSTS) */
#define LPSTS_SUSPM	0x4000

/* USB General control register 2 (UGCTRL2), bit[31:6] should be 0 */
#define UGCTRL2_RESERVED_3	0x00000001	/* bit[3:0] should be B'0001 */
#define UGCTRL2_USB0SEL_OTG	0x00000030

void usbhs_write32(struct usbhs_priv *priv, u32 reg, u32 data)
{
	writel(data, priv->base + reg);
}

static int usbhs_rcar3_power_ctrl(struct platform_device *pdev,
				void __iomem *base, int enable)
{
	struct usbhs_priv *priv = usbhs_pdev_to_priv(pdev);

	pr_dbg("++%s\n", __func__);

	usbhs_write32(priv, UGCTRL2, UGCTRL2_RESERVED_3 | UGCTRL2_USB0SEL_OTG);

	if (enable)
		usbhs_bset(priv, LPSTS, LPSTS_SUSPM, LPSTS_SUSPM);
	else
		usbhs_bset(priv, LPSTS, LPSTS_SUSPM, 0);

	pr_dbg("--%s\n", __func__);

	return 0;
}

static int usbhs_rcar3_get_id(struct platform_device *pdev)
{
	return USBHS_GADGET;
}

const struct renesas_usbhs_platform_callback usbhs_rcar3_ops = {
	.power_ctrl = usbhs_rcar3_power_ctrl,
	.get_id = usbhs_rcar3_get_id,
};

