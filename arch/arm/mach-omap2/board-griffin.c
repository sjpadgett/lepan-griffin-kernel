/*
 * Copyright (C) 2009-2010 Texas Instruments Inc.
* Modified from mach-omap2/board-ldp.c
* Mikkel Christensen <mlc@ti.com>
 * Felipe Balbi <balbi@ti.com>
 * Modified from mach-omap2/board-zoom/some archos and various other sources
 * modified by sjpadgett@gmail.com Aug 2012
  *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
//CONFIG_CMDLINE="root=/dev/mmcblk0p1 rootwait console=ttyO2,115200 androidboot.mode=normal"
//CONFIG_CMDLINE_FORCE=y

/* TODO: figure out how all this shit works!!!!! */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/gpio.h>
#include <linux/i2c/twl.h>
#include <linux/mtd/nand.h>
#include <linux/memblock.h>
#include <linux/bootmem.h>
#include <linux/skbuff.h>
#include <linux/ti_wilink_st.h>
#include <linux/wl12xx.h>
#include <linux/input/goodix-gt80x.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>

#include <plat/common.h>
#include <plat/board.h>
#include <plat/usb.h>
#include <plat/nand.h>
#include <plat/gpmc.h>
#include "android_composite.h"
#include <mach/board-griffin.h>

#include "board-flash.h"
#include "mux.h"
//#include "sdram-micron-mt46h32m32lf-6.h"
#include "sdram-hynix-h8mbx00u0mer-0em.h"
#include "omap_ion.h"
#include "timer-gp.h"

#define GRIFFIN_EHCI_RESET_GPIO		164
#define GRIFFIN_McBSP3_BT_GPIO            156
#define GRIFFIN_BT_RESET_GPIO             156
#define GRIFFIN_WIFI_PMENA_GPIO		186
#define GRIFFIN_WIFI_IRQ_GPIO		152
#define GRIFFIN_LCD_3V3_EN_GPIO		181

#define WILINK_UART_DEV_NAME            "/dev/ttyO1"

extern struct omap_board_mux *griffin_board_mux_ptr;
extern struct omap_board_mux *sec_board_wk_mux_ptr;
/*
 * NAND Layout:
 * 00000000 - 0007FFFF SIZE = 00080000, 512KB : MLO, 4 copies same image. Each of them 128K.
 * 00080000 - 001FFFFF SIZE = 00180000, 1536KB : u-boot, 3 copies same image, each of them 512K.
 * 00200000 - 00C1FFFF SIZE = 00A20000, 10368KB : Probably for "innocomm"
 * 00C20000 - 00FDFFFF SIZE = 003C0000, 3840KB : Recovery partition
 * 00FE0000 - 011DFFFF SIZE = 00200000, 2048KB : RAMDISK
 * 011e0000 - 0159FFFF SIZE = 003C0000, 3840KB : Linux kernel
 * 015A0000 - 016DFFFF SIZE = 00140000, 1280KB : Probably "misc and kpanic" based on sizes
 * 016E0000 - 116DFFFF SIZE = 10000000, 256MB : System Partion
 * 116E0000 - 11A9FFFF SIZE = 003C0000, 3840KB : Android Cache
 * 11AA0000 - 1FFFFFFF SIZE = 0e560000, 229MB : Data partition on NAND, not used.*/

// static struct mtd_partition griffin_nand_partitions[] = {
//     /* All the partition sizes are listed in terms of NAND block size */
//     {
// 	  .name		= "iload",
// 	  .offset		= 0,
// 	  .size		= 4 * (64 * 2048),	/* 512KB, 0x80000 */
// 	  .mask_flags	= MTD_WRITEABLE,	/* force read-only */
//     },
//     {
// 	  .name		= "iboot",
// 	  .offset		= MTDPART_OFS_APPEND,	/* Offset = 0x80000 */
// 	  .size		= 0x180000,	/* 1  */
// 	  .mask_flags	= MTD_WRITEABLE,	/* force read-only */
//     },
//     {
// 	  .name		= "boardenv",
// 	  .offset		= MTDPART_OFS_APPEND,   /* Offset =  */
// 	  .size		= 0xA20000,	/*  */
//     },
//     {
// 	  .name		= "recovery",
// 	  .offset		= MTDPART_OFS_APPEND,	/* Offset = 0x0C20000*/
// 	  .size		= 0x3C0000,	/*  */
//     },
//     {
// 	  .name		= "rootfs",
// 	  .offset		= MTDPART_OFS_APPEND,	/*  */
// 	  .size		=0x200000 ,	/*  */
//     },
//     {
// 	  .name		= "kernel",
// 	  .offset		= MTDPART_OFS_APPEND,	/* */
// 	  .size		= 0x3C0000,	/*  */
//     },
//     {
// 	  .name		= "misc",
// 	  .offset		= MTDPART_OFS_APPEND,	/* */
// 	  .size		= 0x140000,	/*  */
//     },
//     {
// 	  .name		= "system",
// 	  .offset		= MTDPART_OFS_APPEND,	/* */
// 	  .size		= 0x10000000,	/*  */
//     },
//     {
// 	  .name		= "cache",
// 	  .offset		= MTDPART_OFS_APPEND,	/* */
// 	  .size		= 0x3C0000,	/*  */
//     },
//     {
// 	  .name		= "userdata",
// 	  .offset		= MTDPART_OFS_APPEND,	/* Offset = 0x11AA0000*/
// 	  .size		= MTDPART_SIZ_FULL,  //0xe560000,	/*  */
//     },
// };

#ifdef CONFIG_USB_ANDROID
    #define GOOGLE_VENDOR_ID		0x22A4
    #define GOOGLE_PRODUCT_ID		0x1052
    #define GOOGLE_ADB_PRODUCT_ID		0x1052

static char *usb_functions_adb[] = {
	"adb",
};

static char *usb_functions_mass_storage[] = {
	"usb_mass_storage",
};
static char *usb_functions_ums_adb[] = {
	"usb_mass_storage",
	"adb",
};

static char *usb_functions_all[] = {
	"adb", "usb_mass_storage",
};

static struct android_usb_product usb_products[] = {
	{
		.product_id	= GOOGLE_PRODUCT_ID,
		.num_functions	= ARRAY_SIZE(usb_functions_adb),
		.functions	= usb_functions_adb,
	},
	{
		.product_id	= GOOGLE_PRODUCT_ID,
		.num_functions	= ARRAY_SIZE(usb_functions_mass_storage),
		.functions	= usb_functions_mass_storage,
	},
	{
		.product_id	= GOOGLE_PRODUCT_ID,
		.num_functions	= ARRAY_SIZE(usb_functions_ums_adb),
		.functions	= usb_functions_ums_adb,
	},
};

static struct usb_mass_storage_platform_data mass_storage_pdata = {
	.nluns		= 1,
	.vendor		= "jerrys",
	.product	= "jerrys gadget",
	.release	= 0x100,
};

static struct platform_device usb_mass_storage_device = {
	.name	= "usb_mass_storage",
	.id	= -1,
	.dev	= {
		.platform_data = &mass_storage_pdata,
	},
};

static struct android_usb_platform_data android_usb_pdata = {
	.vendor_id	= GOOGLE_VENDOR_ID,
	.product_id	= GOOGLE_PRODUCT_ID,
	.functions	= usb_functions_all,
	.num_products	= ARRAY_SIZE(usb_products),
	.products	= usb_products,
	.version	= 0x0100,
	.product_name	= "jerrys gadget",
	.manufacturer_name	= "jerrys",
	.serial_number	= "20100720",
	.num_functions	= ARRAY_SIZE(usb_functions_all),
};

static struct platform_device androidusb_device = {
	.name	= "android_usb",
	.id	= -1,
	.dev	= {
		.platform_data = &android_usb_pdata,
	},
};

static void griffin_android_gadget_init(void)
{
	platform_device_register(&androidusb_device);
}

#endif

static const struct usbhs_omap_board_data usbhs_bdata __initconst = {
	.port_mode[0]		= OMAP_USBHS_PORT_MODE_UNUSED,
	.port_mode[1]		= OMAP_EHCI_PORT_MODE_PHY,
	.port_mode[2]		= OMAP_USBHS_PORT_MODE_UNUSED,
	.phy_reset		= true,
	.reset_gpio_port[0]	= -EINVAL,
	.reset_gpio_port[1]	= GRIFFIN_EHCI_RESET_GPIO,
	.reset_gpio_port[2]	= -EINVAL,
};

/* Board initialization */
static struct omap_board_config_kernel griffin_config[] __initdata = {
};

static int plat_kim_suspend(struct platform_device *pdev, pm_message_t state)
{
	/* TODO: wait for HCI-LL sleep */
	return 0;
}
static int plat_kim_resume(struct platform_device *pdev)
{
	return 0;
}

/* wl127x BT, FM, GPS connectivity chip */
struct ti_st_plat_data wilink_pdata = {
	.nshutdown_gpio = 186,
	.dev_name = WILINK_UART_DEV_NAME,
	.flow_cntrl = 1,
	.baud_rate = 3686400,
	.suspend = plat_kim_suspend,
	.resume = plat_kim_resume,
};
static struct platform_device wl127x_device = {
	.name           = "kim",
	.id             = -1,
	.dev.platform_data = &wilink_pdata,
};
static struct platform_device btwilink_device = {
	.name = "btwilink",
	.id = -1,
};

static struct platform_device *griffin_devices[] __initdata = {
	&wl127x_device,
	&btwilink_device,
};

/* Fix to prevent VIO leakage on wl127x */
static int wl127x_vio_leakage_fix(void)
{
	int ret = 0;

	pr_info(" wl127x_vio_leakage_fix\n");

	ret = gpio_request(GRIFFIN_BT_RESET_GPIO, "wl127x_bten");
	if (ret < 0) {
		pr_err("wl127x_bten gpio_%d request fail",
			GRIFFIN_BT_RESET_GPIO);
		goto fail;
	}
	gpio_direction_output(GRIFFIN_BT_RESET_GPIO, 1);
	mdelay(10);
	gpio_direction_output(GRIFFIN_BT_RESET_GPIO, 0);
	udelay(64);
	gpio_free(GRIFFIN_BT_RESET_GPIO);
	pr_info(" wl127x_vio_leakage_fix Success!!\n");
fail:
	return ret;
}

static void config_wlan_mux(void)
{
	/* WLAN PW_EN and IRQ */
	omap_mux_init_gpio(GRIFFIN_WIFI_PMENA_GPIO, OMAP_PIN_OUTPUT);
	omap_mux_init_gpio(GRIFFIN_WIFI_IRQ_GPIO, OMAP_PIN_INPUT |
				OMAP_PIN_OFF_WAKEUPENABLE);
	/* MMC3 */  
	omap_mux_init_signal("mcspi1_cs2.sdmmc3_clk", OMAP_PIN_INPUT_PULLUP);
	omap_mux_init_signal("mcspi1_cs1.sdmmc3_cmd", OMAP_PIN_INPUT_PULLUP);
	omap_mux_init_signal("sdmmc2_dat4.sdmmc3_dat0", OMAP_PIN_INPUT_PULLUP);
	omap_mux_init_signal("sdmmc2_dat5.sdmmc3_dat1", OMAP_PIN_INPUT_PULLUP);
	omap_mux_init_signal("sdmmc2_dat6.sdmmc3_dat2", OMAP_PIN_INPUT_PULLUP);
	omap_mux_init_signal("sdmmc2_dat7.sdmmc3_dat3", OMAP_PIN_INPUT_PULLUP);
}

static struct wl12xx_platform_data griffin_wlan_data __initdata = {
	.irq = OMAP_GPIO_IRQ(GRIFFIN_WIFI_IRQ_GPIO),
	.board_ref_clock = WL12XX_REFCLOCK_26,
	.board_tcxo_clock = WL12XX_TCXOCLOCK_26,
};

static void griffin_wifi_init(void)
{
	config_wlan_mux();
	printk(KERN_INFO "********** wifi init **********\n");
	if (wl12xx_set_platform_data(&griffin_wlan_data))
		pr_err("Error setting wl12xx data\n");
}

static void __init griffin_reserve(void){
  /* do the static reservations first */
	memblock_remove(OMAP3_PHYS_ADDR_SMC_MEM, PHYS_ADDR_SMC_SIZE);
//	printk(KERN_INFO "memblock done\n");
	omap_ion_init();
	printk(KERN_INFO "omap_reserve\n");
	omap_reserve();
}
#ifdef CONFIG_ANDROID_RAM_CONSOLE
static inline void omap2_ramconsole_reserve_sdram(void)
{
    reserve_bootmem(RAM_CONSOLE_START,RAM_CONSOLE_SIZE, 0);
}
#else
static inline void omap2_ramconsole_reserve_sdram(void) {}
#endif /* CONFIG_ANDROID_RAM_CONSOLE */

static void __init griffin_map_io(void){
     printk(KERN_INFO "ram console reserve sdram\n");
     omap2_ramconsole_reserve_sdram();
     omap3_map_io();
}
static void __init griffin_init_irq(void){
    omap_init_irq();
    #ifdef CONFIG_OMAP_32K_TIMER
    omap2_gp_clockevent_set_gptimer(1);
    #endif
}
static void __init omap_griffin_init_early(void){
    omap2_init_common_infrastructure();
    omap2_init_common_devices(h8mbx00u0mer0em_sdrc_params,
					h8mbx00u0mer0em_sdrc_params);
}
struct omap_vout_config {
	u16 max_width;
	u16 max_height;
	u32 max_buffer_size;
	u8  num_buffers;
	u8  num_devices;
	int device_ids[2]; /* -1 for any videoX */
};

static struct omap_vout_config griffin_vout_platform_data = {
	.max_width = 1024,
	.max_height = 768,
	.max_buffer_size = 0x180000,
	.num_buffers = 8,
	.num_devices = 1,
	.device_ids = {1},
};

static struct platform_device griffin_vout_device = {
	.name = "omapvout",
	.id = -1,
	.dev = {
		.platform_data = &griffin_vout_platform_data,
	},
};
static void __init griffin_vout_init(void)
{
	platform_device_register(&griffin_vout_device);
}

static struct platform_device griffin_sgx_device = {
	.name                   = "pvrsrvkm",
	.id             = -1,
};
static struct platform_device griffin_omaplfb_device = {
	.name			= "omaplfb",
	.id			= -1,
};

static void __init griffin_sgx_init(void)
{
	platform_device_register(&griffin_sgx_device);
	platform_device_register(&griffin_omaplfb_device);
}
#define GRIFFIN_NAND_CS    0
static void __init board_griffin_init(void){
    omap3_mux_init(griffin_board_mux_ptr, OMAP_PACKAGE_CBP);
    omap_mux_init_gpio(GRIFFIN_EHCI_RESET_GPIO, OMAP_PIN_OUTPUT);
    omap_mux_init_gpio(GRIFFIN_McBSP3_BT_GPIO, OMAP_PIN_OUTPUT);
    /* Ensure SDRC pins are mux'd for self-refresh */
    omap_mux_init_signal("sdrc_cke0", OMAP_PIN_OUTPUT);
    omap_mux_init_signal("sdrc_cke1", OMAP_PIN_OUTPUT);
//     board_nand_init(griffin_nand_partitions, ARRAY_SIZE(griffin_nand_partitions),
// 			  GRIFFIN_NAND_CS, NAND_BUSWIDTH_16);

    /* Ensure msecure is mux'd to be able to set the RTC. */
    printk(KERN_INFO "********** msecure init gpio_157 **********\n");
    omap_mux_init_signal("mcbsp1_fsr.gpio_157", OMAP_PIN_OFF_OUTPUT_HIGH);
    omap_mux_init_signal("mcbsp1_fsr.gpio_157", OMAP_PIN_OUTPUT);
    gpio_request(157, "msecure");
    gpio_direction_output(157, 1);
    usbhs_init(&usbhs_bdata);
    omap_board_config = griffin_config;
    omap_board_config_size = ARRAY_SIZE(griffin_config);
    griffin_wifi_init();
    griffin_peripherals_init();
    griffin_android_gadget_init();
    griffin_display_init();
//    griffin_vout_init();
//    griffin_sgx_init();
    omap_register_ion();
    platform_add_devices(griffin_devices, ARRAY_SIZE(griffin_devices))
    
    wl127x_vio_leakage_fix();
    
}

// ********* Look at OMAP3_EVM for backlight solution
MACHINE_START(GRIFFIN, "griffin")
        .boot_params = 0x80000100,
	.reserve	        = griffin_reserve,
	.map_io		= griffin_map_io,
	.init_early	= omap_griffin_init_early,
	.init_irq	        = griffin_init_irq,
	.init_machine	= board_griffin_init,
	.timer		= &omap_timer,
MACHINE_END
