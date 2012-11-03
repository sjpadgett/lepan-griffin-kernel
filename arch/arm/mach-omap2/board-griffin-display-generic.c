/*
 * Copyright (C) 2010 Texas Instruments Inc.
 *
 * Modified from mach-omap2/board-zoom-peripherals.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/i2c/twl.h>
#include <linux/spi/spi.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/consumer.h>
#include <plat/mcspi.h>
#include <video/omapdss.h>
#include <video/omap-panel-generic-dpi.h>
#include <plat/omap-pm.h>
#include "mux.h"

#define GRIFFIN_GPIO_LCD_EN 181
#define GRIFFIN_GPIO_LCD_BL 198



/*
 * PWMA/B register offsets (TWL4030_MODULE_PWMA)
 */
#define TWL_INTBR_PMBR1	0xD
#define TWL_INTBR_GPBR1	0xC
#define TWL_LED_PWMON	0x0
#define TWL_LED_PWMOFF	0x1

//TWL4030_MODULE_PWMA
static int griffin_set_bl_intensity(struct omap_dss_device *dssdev, int level)
{
	unsigned char c;
	u8 mux_pwm, enb_pwm;

	if (level > 100)
		return -1;


	twl_i2c_read_u8(TWL4030_MODULE_INTBR, &mux_pwm, TWL_INTBR_PMBR1);
	twl_i2c_read_u8(TWL4030_MODULE_INTBR, &enb_pwm, TWL_INTBR_GPBR1);

	if (level == 0) {
		/* disable pwm0 output and clock */
		enb_pwm = enb_pwm & 0xFA;
		/* change pwm0 pin to gpio pin */
		mux_pwm = mux_pwm & 0xFB;
		twl_i2c_write_u8(TWL4030_MODULE_INTBR,
					enb_pwm, TWL_INTBR_GPBR1);
		twl_i2c_write_u8(TWL4030_MODULE_INTBR,
					mux_pwm, TWL_INTBR_PMBR1);
		return 0;
	}
	if (!((enb_pwm & 0x5) && (mux_pwm & 0x4))) {
		/* change gpio pin to pwm0 pin */
		mux_pwm = mux_pwm | 0x04;
		/* enable pwm1 output and clock*/
		enb_pwm = enb_pwm | 0x05;
		twl_i2c_write_u8(TWL4030_MODULE_INTBR,
					mux_pwm, TWL_INTBR_PMBR1);
		twl_i2c_write_u8(TWL4030_MODULE_INTBR,
					enb_pwm, TWL_INTBR_GPBR1);
	}
	c = ((50 * (100 - level)) / 100) + 1;
	twl_i2c_write_u8(TWL4030_MODULE_PWM0, 0x7F, TWL_LED_PWMOFF);
	twl_i2c_write_u8(TWL4030_MODULE_PWM0, c, TWL_LED_PWMON);

	return 0;

}
static struct gpio griffin_lcd_gpios[] __initdata = {
    { GRIFFIN_GPIO_LCD_EN, GPIOF_OUT_INIT_HIGH, "GRIFFIN_GPIO_LCD_EN" },
 //   { GRIFFIN_GPIO_LCD_BL, GPIOF_OUT_INIT_HIGH, "GRIFFIN_GPIO_LCD_BL" },
};

static void __init griffin_lcd_panel_init(void)
{
    if (gpio_request_array(griffin_lcd_gpios, ARRAY_SIZE(griffin_lcd_gpios))) {
	printk(KERN_ERR "could not obtain DSS control GPIOs\n");
	return;
    }
/*    
    gpio_export(GRIFFIN_GPIO_LCD_EN, 0);
    gpio_export(GRIFFIN_GPIO_LCD_BL, 0);*/

}

static int griffin_panel_enable_lcd(struct omap_dss_device *dssdev)
{
//     gpio_set_value_cansleep(GRIFFIN_GPIO_LCD_EN, 1);
//     gpio_set_value_cansleep(GRIFFIN_GPIO_LCD_BL, 1);
    return 0;
}

static void griffin_panel_disable_lcd(struct omap_dss_device *dssdev)
{
//     gpio_set_value_cansleep(GRIFFIN_GPIO_LCD_EN, 0);
//     gpio_set_value_cansleep(GRIFFIN_GPIO_LCD_BL, 0);
}
static struct panel_generic_dpi_data griffin_panel = {
    .name			= "griffin_lp_panel",
    .platform_enable	= griffin_panel_enable_lcd,
    .platform_disable	= griffin_panel_disable_lcd,
   // .set_backlight		= griffin_set_bl_intensity,
};

static struct omap_dss_device griffin_lcd_device = {
    .name			= "lcd",
    .type			= OMAP_DISPLAY_TYPE_DPI,
	.clocks = {
		.dispc  = {
			.dispc_fclk_src = OMAP_DSS_CLK_SRC_DSI_PLL_HSDIV_DISPC,
		},
	},
    .driver_name		= "generic_dpi_panel",
    .data			= &griffin_panel,
    .phy.dpi.data_lines	= 18,
    .channel		= OMAP_DSS_CHANNEL_LCD,
};


static struct omap_dss_device *griffin_dss_devices[] = {
	&griffin_lcd_device,
};

static struct omap_dss_board_info griffin_dss_data = {
	.num_devices		= ARRAY_SIZE(griffin_dss_devices),
	.devices		= griffin_dss_devices,
	.default_device		= &griffin_lcd_device,
};


void __init griffin_display_init(void)
{
	omap_display_init(&griffin_dss_data);
	griffin_lcd_panel_init();

}

