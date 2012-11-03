/*
 * Defines for griffin boards
 */
#include <video/omapdss.h>
#include <linux/input/goodix-gt80x.h>

#define GRIFFIN_NAND_CS    0
extern void __init griffin_peripherals_init(void);
extern void __init griffin_display_init(void);

#if (defined(CONFIG_VIDEO_IMX046) || defined(CONFIG_VIDEO_IMX046_MODULE)) && \
	defined(CONFIG_VIDEO_OMAP3)
#include <media/imx046.h>
extern struct imx046_platform_data zoom2_imx046_platform_data;
#endif

#ifdef CONFIG_VIDEO_OMAP3
extern void griffin_cam_init(void);
#else
#define griffin_cam_init()	NULL
#endif

#define GRIFFIN_HEADSET_EXTMUTE_GPIO	158

extern int has_1GHz_support(void);

struct griffin_i2c_tsp_config
{
	int irq_gpio;
	char *irq_signal;
	int pwr_gpio;
	char *pwr_signal;
	int shtdwn_gpio;
	char *shtdwn_signal;
};
extern int __init griffin_touchscreen_goodix_init(struct goodix_gt80x_platform_data *pdata);

// struct goodix_gt80x_platform_data {
// 	int irq;
// 
// 	int flags;
// 	int orientation;
// 	int init_version;
// 
// 	void (*set_power)(int on_off);
// 	void (*set_shutdown)(int on_off);
// };



