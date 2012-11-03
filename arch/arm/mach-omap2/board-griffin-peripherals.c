/*
 * Copyright (C) 2009 Texas Instruments Inc.
 *
 * Modified from mach-omap2/board-ZOOM2.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/input/matrix_keypad.h>
#include <linux/gpio.h>
#include <linux/i2c/twl.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/fixed.h>
#include <linux/wl12xx.h>
#include <linux/mmc/host.h>
#include <linux/leds-omap4430sdp-display.h>
#include <linux/input/goodix-gt80x.h>

#include <media/v4l2-int-device.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <plat/common.h>
#include <plat/usb.h>
#include <linux/switch.h>
#include <mach/board-griffin.h>
#include "mux.h"
#include "hsmmc.h"
#include "common-board-devices.h"
#include "twl4030.h"
#include "control.h"   /* Swiped some defines from #include <plat/control.h>  to
cleanup some compiler warnings */

#define OMAP_GRIFFIN_WLAN_PMENA_GPIO	(186)
#define OMAP_GRIFFIN_WLAN_IRQ_GPIO		(152)
#define INNOCOMM_TP_IRQ_GPIO			(153)
#define INNOCOMM_TK_IRQ_GPIO			(154)
#define INST_GS3V3_GPIO			(182)
#define FLASH_EMMC_EN_GPIO			(35)
#define GRIFFIN_LCD_3V3_EN_GPIO		(181)
#define LCD_PANEL_ENABLE_GPIO			(6 + OMAP_MAX_GPIO_LINES)

/* PWM output/clock enable for LCD backlight*/
#define REG_INTBR_GPBR1				(0xc)
#define REG_INTBR_GPBR1_PWM0_OUT_EN		(0x1 <<  2)
#define REG_INTBR_GPBR1_PWM0_OUT_EN_MASK	(0x1 <<  2)
#define REG_INTBR_GPBR1_PWM0_CLK_EN		(0x1)
#define REG_INTBR_GPBR1_PWM0_CLK_EN_MASK	(0x1)

/* pin mux for LCD backlight*/
#define REG_INTBR_PMBR1				(0xd)
#define REG_INTBR_PMBR1_PWM0_PIN_EN		(0x1 <<  2)
#define REG_INTBR_PMBR1_PWM0_PIN_MASK		(0x1 <<  2)
#define MAX_CYCLES				(0x7f)
#define MIN_CYCLES				(75)


/* CONTROL_PROG_IO1 bits on omap3630 */
#define OMAP3630_PRG_I2C2_PULLUPRESX    (1 << 0)
#define OMAP3630_PRG_I2C1_PULLUPRESX	(1 << 19)
/* CONTROL_PROG_IO2 bits on omap3630 */
#define OMAP3630_PRG_I2C3_PULLUPRESX    (1 << 7)
/* CONTROL_PROG_IO_WKUP1 bits on omap3630 */
#define OMAP3630_PRG_SR_PULLUPRESX      (1 << 5)

/* VOLUME UP/DOWN */
static int board_keymap[] = {
    KEY(0, 0, KEY_VOLUMEUP),
    KEY(0, 1, KEY_VOLUMEDOWN),
};
static struct matrix_keymap_data board_map_data = {
    .keymap		= board_keymap,
    .keymap_size	= ARRAY_SIZE(board_keymap),
};
static struct twl4030_keypad_data griffin_kp_twl4030_data = {
    .keymap_data	= &board_map_data,
    .rows		= 1,
    .cols		= 2,
    .rep		= 1,
};

static struct __initdata twl4030_power_data griffin_t2scripts_data;
static struct gpio_switch_platform_data headset_switch_data = {
    .name		= "h2w",
    .gpio		= OMAP_MAX_GPIO_LINES + 2, /* TWL4030 GPIO_2 */
};
static struct platform_device headset_switch_device = {
    .name		= "switch-gpio",
    .id		= 0,
    .dev		= {
	  .platform_data = &headset_switch_data,
    },
};
static int griffin_batt_table[] = {
    /* 0 C*/
    30800, 29500, 28300, 27100,
    26000, 24900, 23900, 22900, 22000, 21100, 20300, 19400, 18700, 17900,
    17200, 16500, 15900, 15300, 14700, 14100, 13600, 13100, 12600, 12100,
    11600, 11200, 10800, 10400, 10000, 9630,  9280,  8950,  8620,  8310,
    8020,  7730,  7460,  7200,  6950,  6710,  6470,  6250,  6040,  5830,
    5640,  5450,  5260,  5090,  4920,  4760,  4600,  4450,  4310,  4170,
    4040,  3910,  3790,  3670,  3550
};

static struct twl4030_bci_platform_data griffin_bci_data = {
    .battery_tmp_tbl	= griffin_batt_table,
    .tblsize		= ARRAY_SIZE(griffin_batt_table),
};

/* ***** Consumer Regulators ************ */

static struct regulator_consumer_supply griffin_vmmc1_supply = {
    .supply		= "vmmc",
    //.dev_name	= "omap_hsmmc.0",
};

static struct regulator_consumer_supply griffin_vmmc2_supply = {
    .supply		= "camera_2V85",
    //.dev_name	= "omap3isp",
};

static struct regulator_consumer_supply griffin_vmmc3_supply = {
    .supply		= "emmc",
    .dev_name	= "omap_hsmmc.2",
};

static struct regulator_consumer_supply griffin_vsim_supply = {
    .supply		= "vmmc_aux",
    // .dev_name	= "soc-audio",
};

static struct regulator_consumer_supply griffin_vpll2_supplies[] = {
    REGULATOR_SUPPLY("vdds_dsi", "omapdss"),
    REGULATOR_SUPPLY("vdds_dsi", "omapdss_dsi1"),
//     REGULATOR_SUPPLY("vdds_dsi","display0"),
//     REGULATOR_SUPPLY( "vdds_dsi","omapdss"),
};

static struct regulator_consumer_supply griffin_vdac_supplies[] = {
    REGULATOR_SUPPLY("i2c2_1v8", "2-002f"),
    REGULATOR_SUPPLY("vio_ecompass", "ecompass_pm"),
    REGULATOR_SUPPLY("vio_gsensor",  "gsensor_pm"),
    REGULATOR_SUPPLY("vio_light", "light_pm"),
    REGULATOR_SUPPLY("vio_ov_module", "omap3isp"),
    REGULATOR_SUPPLY("vio_tk",  "tk_pm"),
    REGULATOR_SUPPLY("vio_touch", "touch_pm"),
};

static struct regulator_consumer_supply griffin_vaux1_supply =
REGULATOR_SUPPLY("VIB", NULL);

static struct regulator_consumer_supply griffin_vaux2_supply =
REGULATOR_SUPPLY("gps", NULL);

static struct regulator_consumer_supply griffin_vaux3_supply =
REGULATOR_SUPPLY("aux3_cam_1v8", NULL);

static struct regulator_consumer_supply griffin_vaux4_supply =
REGULATOR_SUPPLY("aux4_cam_1v8", NULL);

/* ***** Regulators Data Init ************ */
// sdcard 
static struct regulator_init_data griffin_vmmc1 = {
    .constraints = {
	  .min_uV			= 1850000,
	  .max_uV			= 3150000,
//	  .boot_on = 1,
	  .valid_modes_mask	= REGULATOR_MODE_NORMAL | REGULATOR_MODE_STANDBY,
	  .valid_ops_mask		= REGULATOR_CHANGE_VOLTAGE |
	  REGULATOR_CHANGE_MODE | REGULATOR_CHANGE_STATUS,
    },
    .num_consumer_supplies  = 1,
    .consumer_supplies      = &griffin_vmmc1_supply,
};
// camera 2v85
static struct regulator_init_data griffin_vmmc2 = {
    .constraints = {
	  .min_uV			= 2850000,
	  .max_uV			= 2850000,
	  .apply_uV		= true,
	  .valid_modes_mask	= REGULATOR_MODE_NORMAL
	  | REGULATOR_MODE_STANDBY,
	  .valid_ops_mask		= REGULATOR_CHANGE_MODE
	  | REGULATOR_CHANGE_STATUS,
    },
    .num_consumer_supplies  = 1,
    .consumer_supplies      = &griffin_vmmc2_supply,
};
/* Fixed regulator internal to Wifi module */
static struct regulator_init_data griffin_vmmc3 = {
    .constraints = {
	  .valid_ops_mask	= REGULATOR_CHANGE_STATUS,
    },
    .num_consumer_supplies	= 1,
    .consumer_supplies = &griffin_vmmc3_supply,
};
// VSIM = ear phone jack
static struct regulator_init_data griffin_vsim = {
    .constraints = {
	  .min_uV			= 3000000,
	  .max_uV			= 3000000,
	  .apply_uV		= true,
	  .valid_modes_mask	= REGULATOR_MODE_NORMAL
	  | REGULATOR_MODE_STANDBY,
	  .valid_ops_mask		= REGULATOR_CHANGE_MODE
	  | REGULATOR_CHANGE_STATUS,
    },
    .num_consumer_supplies  = 1,
    .consumer_supplies      = &griffin_vsim_supply,
};
/* VPLL2 for digital video outputs */
static struct regulator_init_data griffin_vpll2 = {
    .constraints = {
	  //	.name			= "VPLL2",
	  .min_uV                 = 1800000,
	  .max_uV                 = 1800000,
	  .valid_modes_mask       = REGULATOR_MODE_NORMAL |
	  REGULATOR_MODE_STANDBY,
	  .valid_ops_mask  = REGULATOR_CHANGE_MODE | REGULATOR_CHANGE_STATUS,
    },
    .num_consumer_supplies		= ARRAY_SIZE(griffin_vpll2_supplies),
    .consumer_supplies		=  griffin_vpll2_supplies,
};
/* VDAC for  Instrumentation etc */
static struct regulator_init_data griffin_vdac = {
    .constraints = {
	  //	.name			= "VDAC",
	  .min_uV                 = 1800000,
	  .max_uV                 = 1800000,
	  .apply_uV		= true,
	  .valid_modes_mask       = REGULATOR_MODE_NORMAL
	  | REGULATOR_MODE_STANDBY,
	  .valid_ops_mask         = REGULATOR_CHANGE_MODE
	  | REGULATOR_CHANGE_STATUS,
    },
    .num_consumer_supplies		= ARRAY_SIZE(griffin_vdac_supplies),
    .consumer_supplies		=  griffin_vdac_supplies,
};
/* VAUX1 for CAM_1V8 */
static struct regulator_init_data griffin_vaux1 = {
    .constraints = {
	  //	.name			= "VAUX1",
	  .min_uV                 = 3000000,
	  .max_uV                 = 3000000,
	  .apply_uV		= true,
	  .valid_modes_mask       = REGULATOR_MODE_NORMAL
	  | REGULATOR_MODE_STANDBY,
	  .valid_ops_mask         = REGULATOR_CHANGE_MODE
	  | REGULATOR_CHANGE_STATUS,
    },
    .num_consumer_supplies		= 1,
    .consumer_supplies		= &griffin_vaux1_supply,
};
/* VAUX2 for CAM_1V8 */
static struct regulator_init_data griffin_vaux2 = {
    .constraints = {
	  //	.name			= "VAUX2",
	  .min_uV                 = 1800000,
	  .max_uV                 = 1800000,
	  .apply_uV		= true,
	  .valid_modes_mask       = REGULATOR_MODE_NORMAL
	  | REGULATOR_MODE_STANDBY,
	  .valid_ops_mask         = REGULATOR_CHANGE_MODE
	  | REGULATOR_CHANGE_STATUS,
    },
    .num_consumer_supplies		= 1,
    .consumer_supplies		= &griffin_vaux2_supply,
};
/* VAUX3 for CAM_1V8 */
static struct regulator_init_data griffin_vaux3 = {
    .constraints = {
	  .min_uV                 = 1800000,
	  .max_uV                 = 1800000,
	  .apply_uV               = true,
	  .valid_modes_mask       = REGULATOR_MODE_NORMAL
	  | REGULATOR_MODE_STANDBY,
	  .valid_ops_mask         = REGULATOR_CHANGE_MODE
	  | REGULATOR_CHANGE_STATUS,
    },
    .num_consumer_supplies  = 1,
    .consumer_supplies      = &griffin_vaux3_supply,
};
/* VAUX4 for CAM_2V8  1v8 for our's*/
static struct regulator_init_data griffin_vaux4 = {
    .constraints = {
	  .min_uV                 = 1800000,
	  .max_uV                 = 1800000,
	  .apply_uV               = true,
	  .valid_modes_mask       = REGULATOR_MODE_NORMAL
	  | REGULATOR_MODE_STANDBY,
	  .valid_ops_mask         = REGULATOR_CHANGE_MODE
	  | REGULATOR_CHANGE_STATUS,
    },
    .num_consumer_supplies  = 1,
    .consumer_supplies      = &griffin_vaux4_supply,
};

/* Fixed regulator for internal wlan */
static struct fixed_voltage_config griffin_vwlan = {
    .supply_name		= "vwl1271",
    .microvolts		= 1800000, /* 1.8V */
    .gpio			= OMAP_GRIFFIN_WLAN_PMENA_GPIO,
    .startup_delay		= 70000, /* 70msec */
    .enable_high		= 1,
    .enabled_at_boot	= 1,
    .init_data		= &griffin_vmmc3,
};
static struct platform_device omap_vwlan_device = {
    .name		= "reg-fixed-voltage",
    .id		= 2,
    .dev = {
	  .platform_data	= &griffin_vwlan,
    },
};

/* Fixed regulator for internal GS_3v3 working regulators see vdac for vio 1v8
 *bus */
static struct regulator_consumer_supply griffin_gs3v3_consumers[] = {
    REGULATOR_SUPPLY("i2c2_2v8", "2-002f"),
    REGULATOR_SUPPLY("ecompass", "ecompass_pm"),
    REGULATOR_SUPPLY("gsensor", "gsensor_pm"),
    REGULATOR_SUPPLY("light", "light_pm"),
    REGULATOR_SUPPLY("ov_module", "omap3isp"),
    REGULATOR_SUPPLY("tk", "tk_pm"),
    REGULATOR_SUPPLY("touch", "touch_pm"),
};
static struct regulator_init_data griffin_gs3v3 = {
    .constraints = {
	  .name			= "GS_3v3",
	  .valid_modes_mask	= REGULATOR_MODE_NORMAL
	  | REGULATOR_MODE_STANDBY,
	  .valid_ops_mask		= REGULATOR_CHANGE_STATUS
	  | REGULATOR_CHANGE_MODE,
    },
    .num_consumer_supplies		= ARRAY_SIZE(griffin_gs3v3_consumers),
    
    .consumer_supplies		=
    griffin_gs3v3_consumers,
};
static struct fixed_voltage_config griffin_gs3v3_config = {
    .supply_name		= "GS_3v3",
    .microvolts		= 3300000,
    .gpio			= INST_GS3V3_GPIO,
    .startup_delay		= 150,
    .enable_high		= 1,
    .init_data		= &griffin_gs3v3,
};
static struct platform_device griffin_gs3v3_device = {
    .name			= "reg-fixed-voltage",
    .id		= 0,
    .dev			= {
	  .platform_data	= &griffin_gs3v3_config,
    },
};

/* Fixed regulator for internal eMMC */
static struct regulator_consumer_supply griffin_vemmc_supply =
REGULATOR_SUPPLY("vmmc",NULL);
static struct regulator_init_data griffin_vemmc = {
    .constraints = {
	  .name			= "eMMC_PWR",
	  .valid_modes_mask	= REGULATOR_MODE_NORMAL
	  | REGULATOR_MODE_STANDBY,
	  .valid_ops_mask		= REGULATOR_CHANGE_STATUS
	  | REGULATOR_CHANGE_MODE,
    },
    .num_consumer_supplies		=  1, //ARRAY_SIZE(griffin_vemmc_supply),
    .consumer_supplies		= &griffin_vemmc_supply,
};
static struct fixed_voltage_config griffin_vemmc_config = {
    .supply_name		= "VEMMC",
    .microvolts		= 3300000,
    .gpio			= FLASH_EMMC_EN_GPIO,
    .startup_delay		= 150,
    .enable_high		= 1,
    .init_data		= &griffin_vemmc,
};
static struct platform_device griffin_vemmc_device = {
    .name			= "reg-fixed-voltage",
    .id		= 1,
    .dev			= {
	  .platform_data	= &griffin_vemmc_config,
    },
};

static struct platform_device *griffin_board_devices[] __initdata = {
    &griffin_gs3v3_device,
    //&headset_switch_device,
    &griffin_vemmc_device,
};

static struct omap2_hsmmc_info mmc[] = {
    {
	  .name		= "external",
	  .mmc		= 1,
	  .caps		= MMC_CAP_4_BIT_DATA,
	  .gpio_cd	   	= -EINVAL,
	  .gpio_wp	= -EINVAL,
	  .power_saving	= true,
    },
    // e.mmc nand
    {
	  .name		= "internal",
	  .mmc		= 2,
	  .caps		= MMC_CAP_8_BIT_DATA | MMC_CAP_NONREMOVABLE,
	  .gpio_cd	   	= -EINVAL,
	  .gpio_wp	= -EINVAL, //164,
	  .nonremovable	= true,
	  .power_saving	= true,
    },
    // wilan
    {
	  .name		= "wl1271",
	  .mmc		= 3,
	  .caps		= MMC_CAP_4_BIT_DATA | MMC_CAP_POWER_OFF_CARD |
	  MMC_PM_KEEP_POWER,
	  .gpio_wp	= -EINVAL,
	  .gpio_cd	        = -EINVAL,
	  .nonremovable	= true,
    },
    {}      /* Terminator */
};

static int griffin_twl_gpio_setup(struct device *dev, unsigned gpio, unsigned ngpio) {
    int ret;
    ret = 0;
    /* gpio + 0 is "mmc0_cd" (input/IRQ) */
    mmc[0].gpio_cd = gpio + 0;

    omap2_hsmmc_init(mmc);
  
    griffin_vmmc1_supply.dev = mmc[0].dev;
    griffin_vsim_supply.dev = mmc[0].dev;
    griffin_vemmc_supply.dev = mmc[1].dev;
   //griffin_vmmc3_supply.dev = mmc[2].dev;
    
//     ret = gpio_request_one(LCD_PANEL_ENABLE_GPIO, GPIOF_OUT_INIT_HIGH,
// 				   "lcd enable");
//     if (ret)
// 	  pr_err("Failed to get LCD_PANEL_ENABLE_GPIO (gpio%d).\n",
// 		   GRIFFIN_LCD_3V3_EN_GPIO);
// 	  
// 	  printk(KERN_INFO "%s:%d ngpio=%d\n", __func__, __LINE__, LCD_PANEL_ENABLE_GPIO);
//          ret = gpio_request_one(38, GPIOF_OUT_INIT_LOW, "VBUS HOST");
//          if (ret){
// 	   	  pr_err("Failed to get VBUS HOST.\n");
//          } 
    return ret;
}

static void griffin_set_hs_extmute(int mute) {
    gpio_set_value(GRIFFIN_HEADSET_EXTMUTE_GPIO, mute);
}

static struct twl4030_usb_data griffin_usb_data = {
    .usb_mode	= T2_USB_MODE_ULPI,
};

static struct twl4030_gpio_platform_data griffin_gpio_data = {
    .gpio_base	= OMAP_MAX_GPIO_LINES,
    .irq_base	= TWL4030_GPIO_IRQ_BASE,
    .irq_end	= TWL4030_GPIO_IRQ_END,
    .setup		= griffin_twl_gpio_setup,
};

static struct twl4030_madc_platform_data griffin_madc_data = {
    .irq_line	= 1,
};

static struct twl4030_codec_audio_data griffin_audio_data;

static struct twl4030_codec_data griffin_codec_data = {
    .audio_mclk = 26000000,
    .audio = &griffin_audio_data,
};

static struct twl4030_platform_data griffin_twldata = {
    .irq_base	= TWL4030_IRQ_BASE,
    .irq_end	= TWL4030_IRQ_END,
    
    /* platform_data for children goes here */
    .bci			= &griffin_bci_data,
    .madc		= &griffin_madc_data,
    .usb			= &griffin_usb_data,
    .gpio		= &griffin_gpio_data,
    .keypad	= &griffin_kp_twl4030_data,
    .codec		= &griffin_codec_data,
    .vmmc1      = &griffin_vmmc1,
    .vmmc2      = &griffin_vmmc2,
    .vsim          	= &griffin_vsim,
    .vdac		= &griffin_vdac,
    .vaux1		= &griffin_vaux1,
    .vaux2		= &griffin_vaux2,
    .vaux3		= &griffin_vaux3,
    .vaux4		= &griffin_vaux4,
    .vpll2		= &griffin_vpll2,
    .power	= &griffin_t2scripts_data,
};

static struct goodix_gt80x_platform_data board_goodix_pdata = {
    .flags = GOODIX_GT80X_FLAGS_XY_SWAP,
    .init_version = 2,
};

static struct i2c_board_info __initdata griffin_i2c_bus2_info[] = {
    {
	  I2C_BOARD_INFO(GOODIX_GT801_NAME, GOODIX_GT801_ADDR),
	  .flags = I2C_CLIENT_WAKE,
	  .platform_data = &board_goodix_pdata,
    },
    
};

static struct i2c_board_info __initdata griffin_i2c_bus3_info[] = {
    // touch keys go here ha2608
};

#include "board-griffin-pwm.c"

#ifdef CONFIG_ANDROID_RAM_CONSOLE
#define RAM_CONSOLE_START   0x8FFE0000
#define RAM_CONSOLE_SIZE    0x20000
static struct resource ram_console_resource = {
    .start  = RAM_CONSOLE_START,
    .end    = (RAM_CONSOLE_START + RAM_CONSOLE_SIZE - 1),
    .flags  = IORESOURCE_MEM,
};
static struct platform_device ram_console_device = {
    .name           = "ram_console",
    .id             = 0,
    .num_resources  = 1,
    .resource       = &ram_console_resource,
};
#endif


static int __init omap_i2c_init(void){
    /* Disable OMAP 3630 internal pull-ups for I2Ci */
    if (cpu_is_omap3630()) {
	  u32 prog_io;
	  prog_io = omap_ctrl_readl(OMAP343X_CONTROL_PROG_IO1);
	  /* Program (bit 19)=1 to disable internal pull-up on I2C1 */
	  prog_io |= OMAP3630_PRG_I2C1_PULLUPRESX;
	  /* Program (bit 0)=1 to disable internal pull-up on I2C2 */
	  prog_io |= OMAP3630_PRG_I2C2_PULLUPRESX;
	  omap_ctrl_writel(prog_io, OMAP343X_CONTROL_PROG_IO1);
	  
	  prog_io = omap_ctrl_readl(OMAP36XX_CONTROL_PROG_IO2);
	  /* Program (bit 7)=1 to disable internal pull-up on I2C3 */
	  prog_io |= OMAP3630_PRG_I2C3_PULLUPRESX;
	  omap_ctrl_writel(prog_io, OMAP36XX_CONTROL_PROG_IO2);
	  
	  prog_io = omap_ctrl_readl(OMAP36XX_CONTROL_PROG_IO_WKUP1);
	  /* Program (bit 5)=1 to disable internal pull-up on I2C4(SR) */
	  prog_io |= OMAP3630_PRG_SR_PULLUPRESX;
	  omap_ctrl_writel(prog_io, OMAP36XX_CONTROL_PROG_IO_WKUP1);
    }
    
    omap_pmic_init(1, 2400, "twl5030", INT_34XX_SYS_NIRQ, &griffin_twldata);
    omap_register_i2c_bus(2, 400, NULL, 0);
    omap_register_i2c_bus(3, 150, NULL, 0);
    // for now no i2c devices
    //     omap_register_i2c_bus(2, 400, griffin_i2c_bus2_info,
    //      			    ARRAY_SIZE(griffin_i2c_bus2_info));
    //     omap_register_i2c_bus(3, 150, griffin_i2c_bus3_info,
    //      			      ARRAY_SIZE(griffin_i2c_bus3_info));

    return 0;
}

static void enable_board_wakeup_source(void){
    /* T2 interrupt line (keypad) */
    omap_mux_init_signal("sys_nirq",
				 OMAP_WAKEUP_EN | OMAP_PIN_INPUT_PULLUP);
}

static struct omap_musb_board_data musb_board_data = {
    .interface_type		= MUSB_INTERFACE_ULPI,
    .mode			= MUSB_PERIPHERAL,
    .power			= 100,
};

void __init griffin_peripherals_init(void){

    twl4030_get_scripts(&griffin_t2scripts_data);
//     if ( gpio_request_one(38, GPIOF_OUT_INIT_LOW, "VBUS HOST"))
// 	  pr_err("Failed to set VBUS HOST.\n");
    #ifdef CONFIG_ANDROID_RAM_CONSOLE
    //printk(KERN_INFO "****** RAM CONSOLE INIT ******\n");
    //platform_device_register(&ram_console_device);
    #endif
    // emmc and gs_3v3 regulators
    platform_add_devices(griffin_board_devices,	 ARRAY_SIZE(griffin_board_devices));
    printk(KERN_INFO "****** i2c init\n");
    omap_i2c_init();
    //griffin_touchscreen_goodix_init(&board_goodix_pdata);
    platform_device_register(&omap_vwlan_device);

    omap_serial_init();
    printk(KERN_INFO "****** musb init\n");
    usb_musb_init(&musb_board_data);
    enable_board_wakeup_source();

   // printk(KERN_INFO "GPIO198 RESET=%d\n", gpio_get_value(198));
    // platform_device_register(&griffin_disp_led);
    
    //    griffin_pwm_enable(0x01);
    //    griffin_pwm_config(63);

    //	griffin_cam_init();
}
