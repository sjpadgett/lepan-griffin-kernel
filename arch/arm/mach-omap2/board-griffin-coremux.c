// Mux configuration taken from device sys/kernel/debug
// todo: cleanup not needed includes

#include <linux/ctype.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <asm/system.h>
#include <plat/control.h>
#include <plat/mux.h>
#include <plat/gpio.h>
#include <linux/interrupt.h>

#include "mux.h"

#define SEC_OMAP_OUTPUT_GPIO(name, val)		{name, val, (unsigned int)#name},

static struct omap_board_mux __omap_board_core_mux[] __initdata = {

/*
 *		Name, reg-offset,
 *		mux-mode | [active-mode | off-mode]
 */
OMAP3_MUX(CAM_D0, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(CAM_D1, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(CAM_D10, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(CAM_D11, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(CAM_D2, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(CAM_D3, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(CAM_D4, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(CAM_D5, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(CAM_D6, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(CAM_D7, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(CAM_D8, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(CAM_D9, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
// hynix cam reset gpio 98
OMAP3_MUX(CAM_FLD, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
//
OMAP3_MUX(CAM_HS, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
OMAP3_MUX(CAM_PCLK, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
OMAP3_MUX(CAM_STROBE, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(CAM_VS, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
// cam  gpio 167  standby
OMAP3_MUX(CAM_WEN, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
//
OMAP3_MUX(CAM_XCLKA, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
OMAP3_MUX(CAM_XCLKB, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(CSI2_DX0, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(CSI2_DX1, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(CSI2_DY0, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(CSI2_DY1, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
// DSS SIG AND CLK
OMAP3_MUX(DSS_ACBIAS, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
OMAP3_MUX(DSS_HSYNC, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
OMAP3_MUX(DSS_PCLK, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
OMAP3_MUX(DSS_VSYNC, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
// DSS DATA 0 - 5 BLUE 0-5
OMAP3_MUX(DSS_DATA18, OMAP_PIN_OUTPUT | OMAP_MUX_MODE3),
OMAP3_MUX(DSS_DATA19, OMAP_PIN_OUTPUT | OMAP_MUX_MODE3),
OMAP3_MUX(DSS_DATA20, OMAP_PIN_OUTPUT | OMAP_MUX_MODE3),
OMAP3_MUX(DSS_DATA21, OMAP_PIN_OUTPUT | OMAP_MUX_MODE3),
OMAP3_MUX(DSS_DATA22, OMAP_PIN_OUTPUT | OMAP_MUX_MODE3),
OMAP3_MUX(DSS_DATA23, OMAP_PIN_OUTPUT | OMAP_MUX_MODE3),
// DSS DATA 6 - 11 GREEN 0-5
OMAP3_MUX(DSS_DATA6, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
OMAP3_MUX(DSS_DATA7, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
OMAP3_MUX(DSS_DATA8, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
OMAP3_MUX(DSS_DATA9, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
OMAP3_MUX(DSS_DATA10, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
OMAP3_MUX(DSS_DATA11, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
// DSS DATA 12 - 17 RED 0-5
OMAP3_MUX(DSS_DATA12, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
OMAP3_MUX(DSS_DATA13, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
OMAP3_MUX(DSS_DATA14, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
OMAP3_MUX(DSS_DATA15, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
OMAP3_MUX(DSS_DATA16, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
OMAP3_MUX(DSS_DATA17, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
// DSS NOT USED
OMAP3_MUX(DSS_DATA0, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(DSS_DATA1, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(DSS_DATA2, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(DSS_DATA3, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(DSS_DATA4, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(DSS_DATA5, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
// ---------
OMAP3_MUX(ETK_CLK, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(ETK_CTL, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(ETK_D0, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(ETK_D1, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(ETK_D10, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
// GPIO's 25 - 29 25=gps reset
OMAP3_MUX(ETK_D11, OMAP_PIN_INPUT | OMAP_MUX_MODE4),
// M RDY
OMAP3_MUX(ETK_D12, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
// S RDY
OMAP3_MUX(ETK_D13, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
// MODEM ALIVE
OMAP3_MUX(ETK_D14, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
// gpio 29 charge detect lo = charging
OMAP3_MUX(ETK_D15, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE4),
// --------------------- 
OMAP3_MUX(ETK_D2, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(ETK_D3, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(ETK_D4, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(ETK_D5, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(ETK_D6, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(ETK_D7, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(ETK_D8, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(ETK_D9, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
// gps on out hi enables gpio 34 was stat1 on sch
OMAP3_MUX(GPMC_A1, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
// emmc pwr on out hi enables gpio 35 was stat2 on sch
OMAP3_MUX(GPMC_A2, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
// gpio 36 ??? 
OMAP3_MUX(GPMC_A3, OMAP_PIN_OUTPUT | OMAP_MUX_MODE4),
// speaker switch out  gpio 37
OMAP3_MUX(GPMC_A4, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
// vbus host enable gpio 38 out lo en
OMAP3_MUX(GPMC_A5, OMAP_PIN_OUTPUT | OMAP_MUX_MODE4),
//  gpiob 39 q24120 en out lo
OMAP3_MUX(GPMC_A6, OMAP_PIN_OUTPUT | OMAP_MUX_MODE4),
// rest unused
OMAP3_MUX(GPMC_A7, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(GPMC_A8, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(GPMC_A9, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(GPMC_A10, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(GPMC_CLK, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
// -----I added d0-d7 and cs0 mux here-  bootloader must have set up not in board mux table
OMAP3_MUX(GPMC_NCS0, OMAP_PIN_OUTPUT| OMAP_MUX_MODE0),
OMAP3_MUX(GPMC_D0, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(GPMC_D1, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(GPMC_D2, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(GPMC_D3, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(GPMC_D4, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(GPMC_D5, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(GPMC_D6, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(GPMC_D7, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
// ---- END D0-D7 or using 8 bit nand
OMAP3_MUX(GPMC_NWP, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(GPMC_NBE0_CLE, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
OMAP3_MUX(GPMC_D8, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(GPMC_D9, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(GPMC_D10, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(GPMC_D11, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(GPMC_D12, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(GPMC_D13, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(GPMC_D14, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(GPMC_D15, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
// ------------------------------
OMAP3_MUX(GPMC_NBE1, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(GPMC_NCS1, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
// gpio's  53,54,55,56 53&54=stat1,stat2 55=fpga done 56=orientation lock
OMAP3_MUX(GPMC_NCS2, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE4),
OMAP3_MUX(GPMC_NCS3, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE4),
OMAP3_MUX(GPMC_NCS4, OMAP_PIN_INPUT | OMAP_MUX_MODE4),
OMAP3_MUX(GPMC_NCS5, OMAP_PIN_INPUT | OMAP_MUX_MODE4),
//------------------------
OMAP3_MUX(GPMC_NCS6, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(GPMC_NCS7, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(GPMC_WAIT1, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(GPMC_WAIT2, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(GPMC_WAIT3, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(HDQ_SIO, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
//
// OMAP3_MUX(HSUSB0_CLK, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
// OMAP3_MUX(HSUSB0_DATA0, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
// OMAP3_MUX(HSUSB0_DATA1, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
// OMAP3_MUX(HSUSB0_DATA2, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
// OMAP3_MUX(HSUSB0_DATA3, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
// OMAP3_MUX(HSUSB0_DATA4, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
// OMAP3_MUX(HSUSB0_DATA5, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
// OMAP3_MUX(HSUSB0_DATA6, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
// OMAP3_MUX(HSUSB0_DATA7, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
// OMAP3_MUX(HSUSB0_DIR, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
// OMAP3_MUX(HSUSB0_NXT, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
// OMAP3_MUX(HSUSB0_STP, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
OMAP3_MUX(I2C2_SCL, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(I2C2_SDA, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(I2C3_SCL, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(I2C3_SDA, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(I2C4_SCL, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
OMAP3_MUX(I2C4_SDA, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
OMAP3_MUX(JTAG_EMU0, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(JTAG_EMU1, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
// bt en gpio 156 kim
OMAP3_MUX(MCBSP1_CLKR, OMAP_PIN_OUTPUT | OMAP_MUX_MODE4),
// gpio 162 iset l2801 q24120 charger out lo en
OMAP3_MUX(MCBSP1_CLKX, OMAP_PIN_OUTPUT | OMAP_MUX_MODE4),
// gpio 159 headset hook irq 319
OMAP3_MUX(MCBSP1_DR, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE4),
//  gpio 158 headset ext mute
OMAP3_MUX(MCBSP1_DX, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
// T5_MSECURE
OMAP3_MUX(MCBSP1_FSR, OMAP_PIN_OUTPUT | OMAP_MUX_MODE4),
//  gpio 161 sch says reset modem 
OMAP3_MUX(MCBSP1_FSX, OMAP_PIN_OUTPUT | OMAP_MUX_MODE4),
// AP_T5_I2S2 TDM AP
OMAP3_MUX(MCBSP2_CLKX, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(MCBSP2_DR, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(MCBSP2_DX, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
OMAP3_MUX(MCBSP2_FSX, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(MCBSP3_CLKX, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(MCBSP3_DR, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(MCBSP3_DX, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(MCBSP3_FSX, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
// gpio 152 wifi irq
OMAP3_MUX(MCBSP4_CLKX, OMAP_PIN_INPUT | OMAP_MUX_MODE4),
// goodix tp and tk irpq
OMAP3_MUX(MCBSP4_DR, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
OMAP3_MUX(MCBSP4_DX, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),
//  USIF_SW
OMAP3_MUX(MCBSP4_FSX, OMAP_PIN_OUTPUT | OMAP_MUX_MODE4),
OMAP3_MUX(MCBSP_CLKS, OMAP_PIN_INPUT | OMAP_MUX_MODE0),

//----------------- sdio3_CMD CLK WL1271 wifi etc
OMAP3_MUX(MCSPI1_CS1, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE3),
OMAP3_MUX(MCSPI1_CS2, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE3),

//----------------- EC RST gpio 177
OMAP3_MUX(MCSPI1_CS3, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),

//-----  inand d4-d7----------------------
OMAP3_MUX(MCSPI1_CLK, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE1),
OMAP3_MUX(MCSPI1_SIMO, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE1),
OMAP3_MUX(MCSPI1_SOMI, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE1),
OMAP3_MUX(MCSPI1_CS0, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE1),

//----------- AP_SPI2_CLK ------------------------------------------
OMAP3_MUX(MCSPI2_CLK, OMAP_PIN_INPUT | OMAP_MUX_MODE0),

//-------- LCD_3V3_EN ------gpio 181????----------------------------
OMAP3_MUX(MCSPI2_CS0, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),

//---------------- GS_3V3_EN  gpio 182 works
OMAP3_MUX(MCSPI2_CS1, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4),

//---------------------------- AP simo somi
OMAP3_MUX(MCSPI2_SIMO, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(MCSPI2_SOMI, OMAP_PIN_INPUT | OMAP_MUX_MODE0),

//-----  sd card ----------------------
OMAP3_MUX(SDMMC1_CLK, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
OMAP3_MUX(SDMMC1_CMD, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
OMAP3_MUX(SDMMC1_DAT0, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
OMAP3_MUX(SDMMC1_DAT1, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
OMAP3_MUX(SDMMC1_DAT2, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
OMAP3_MUX(SDMMC1_DAT3, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
//------------------------------------------------------------------------------------------------
OMAP3_MUX(SIM_IO, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(SIM_CLK, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(SIM_PWRCTRL, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(SIM_RST, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
//-----  enand d0-d3----------------------
OMAP3_MUX(SDMMC2_CLK, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
OMAP3_MUX(SDMMC2_CMD, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
OMAP3_MUX(SDMMC2_DAT0, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
OMAP3_MUX(SDMMC2_DAT1, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
OMAP3_MUX(SDMMC2_DAT2, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
OMAP3_MUX(SDMMC2_DAT3, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
 /* WLAN SDIO: MMC3 DAT[0-3]  */ 
OMAP3_MUX(SDMMC2_DAT4, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE3),
OMAP3_MUX(SDMMC2_DAT5, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE3),
OMAP3_MUX(SDMMC2_DAT6, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE3),
OMAP3_MUX(SDMMC2_DAT7, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE3),
//----sdram
OMAP3_MUX(SDRC_CKE0, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
OMAP3_MUX(SDRC_CKE1, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
OMAP3_MUX(SYS_BOOT0, OMAP_PIN_INPUT | OMAP_MUX_MODE7),
OMAP3_MUX(SYS_BOOT1, OMAP_PIN_INPUT | OMAP_MUX_MODE7),
OMAP3_MUX(SYS_BOOT2, OMAP_PIN_INPUT | OMAP_MUX_MODE7),
OMAP3_MUX(SYS_BOOT3, OMAP_PIN_INPUT | OMAP_MUX_MODE7),
OMAP3_MUX(SYS_BOOT4, OMAP_PIN_INPUT | OMAP_MUX_MODE7),
OMAP3_MUX(SYS_BOOT5, OMAP_PIN_INPUT | OMAP_MUX_MODE7),
OMAP3_MUX(SYS_BOOT6, OMAP_PIN_INPUT | OMAP_MUX_MODE7),
OMAP3_MUX(SYS_CLKOUT1, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
// wifi pmena out hi
OMAP3_MUX(SYS_CLKOUT2, OMAP_PIN_OUTPUT | OMAP_MUX_MODE4),
OMAP3_MUX(SYS_CLKREQ, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(SYS_NIRQ, OMAP_PIN_INPUT_PULLUP | OMAP_PIN_OFF_WAKEUPENABLE | OMAP_MUX_MODE0),
OMAP3_MUX(SYS_NRESWARM, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
OMAP3_MUX(SYS_OFF_MODE, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
// uart1 all btooth
OMAP3_MUX(UART1_CTS, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
OMAP3_MUX(UART1_RTS, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
OMAP3_MUX(UART1_RX, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(UART1_TX, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
// uart2 was gps en and v bus en on sch now unused and gps moved to gpio 34 and 38
OMAP3_MUX(UART2_CTS, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
OMAP3_MUX(UART2_RTS, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7),
// uart2 gps tx rx
OMAP3_MUX(UART2_RX, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
OMAP3_MUX(UART2_TX, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
// vbus host flt in hi
OMAP3_MUX(UART3_CTS_RCTX, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE4),
// emmc-rst out hi
OMAP3_MUX(UART3_RTS_SD, OMAP_PIN_OUTPUT | OMAP_MUX_MODE4),
// uart3 in rx
OMAP3_MUX(UART3_RX_IRRX, OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0),
// uart3 out tx
OMAP3_MUX(UART3_TX_IRTX, OMAP_PIN_OUTPUT | OMAP_MUX_MODE0),
// what? a11 doesnt document mode 1
OMAP3_MUX(GPMC_A11, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE1),
OMAP3_MUX(SAD2D_MBUSFLAG, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(SAD2D_MREAD, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(SAD2D_MWRITE, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(SAD2D_SBUSFLAG, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(SAD2D_SREAD, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(SAD2D_SWRITE, OMAP_PIN_INPUT | OMAP_MUX_MODE0),
OMAP3_MUX(SAD2D_MCAD28, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0),
OMAP3_MUX(SAD2D_MCAD29, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0),
OMAP3_MUX(SAD2D_MCAD32, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0),
OMAP3_MUX(SAD2D_MCAD33, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0),
OMAP3_MUX(SAD2D_MCAD34, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0),
OMAP3_MUX(SAD2D_MCAD35, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0),
OMAP3_MUX(SAD2D_MCAD36, OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0),
{.reg_offset = OMAP_MUX_TERMINATOR},
};

unsigned int griffin_board_mux_size = ARRAY_SIZE(__omap_board_core_mux);
EXPORT_SYMBOL(griffin_board_mux_size);

struct omap_board_mux *griffin_board_mux_ptr __initdata = __omap_board_core_mux;
EXPORT_SYMBOL(griffin_board_mux_ptr);


/*
>>>>>>>>>>>>>>>>>>>>>>>>> From Device for information >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

name: cam_d0.safe_mode (0x48002116/0x0e6 = 0x010f), b ag17, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: cam_d0 | NA | csi2_dx2 | NA | gpio_99 | NA | NA | safe_mode
name: cam_d1.safe_mode (0x48002118/0x0e8 = 0x010f), b ah17, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: cam_d1 | NA | csi2_dy2 | NA | gpio_100 | NA | NA | safe_mode
name: cam_d10.cam_d10 (0x4800212a/0x0fa = 0x0100), b b25, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: cam_d10 | ssi2_wake | NA | NA | gpio_109 | hw_dbg8 | NA | safe_mode
name: cam_d11.cam_d11 (0x4800212c/0x0fc = 0x0100), b c26, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: cam_d11 | NA | NA | NA | gpio_110 | hw_dbg9 | NA | safe_mode
name: cam_d2.cam_d2 (0x4800211a/0x0ea = 0x0100), b b24, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: cam_d2 | ssi2_rdy_tx | NA | NA | gpio_101 | hw_dbg4 | NA | safe_mode
name: cam_d3.cam_d3 (0x4800211c/0x0ec = 0x0100), b c24, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: cam_d3 | ssi2_dat_rx | NA | NA | gpio_102 | hw_dbg5 | NA | safe_mode
name: cam_d4.cam_d4 (0x4800211e/0x0ee = 0x0100), b d24, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: cam_d4 | ssi2_flag_rx | NA | NA | gpio_103 | hw_dbg6 | NA | safe_mode
name: cam_d5.cam_d5 (0x48002120/0x0f0 = 0x0100), b a25, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: cam_d5 | ssi2_rdy_rx | NA | NA | gpio_104 | hw_dbg7 | NA | safe_mode
name: cam_d6.cam_d6 (0x48002122/0x0f2 = 0x0100), b k28, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: cam_d6 | NA | NA | NA | gpio_105 | NA | NA | safe_mode
name: cam_d7.cam_d7 (0x48002124/0x0f4 = 0x0100), b l28, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: cam_d7 | NA | NA | NA | gpio_106 | NA | NA | safe_mode
name: cam_d8.cam_d8 (0x48002126/0x0f6 = 0x0100), b k27, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: cam_d8 | NA | NA | NA | gpio_107 | NA | NA | safe_mode
name: cam_d9.cam_d9 (0x48002128/0x0f8 = 0x0100), b l27, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: cam_d9 | NA | NA | NA | gpio_108 | NA | NA | safe_mode
name: cam_fld.gpio_98 (0x48002114/0x0e4 = 0x010c), b c23, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4
signals: cam_fld | NA | cam_global_reset | NA | gpio_98 | hw_dbg3 | NA | safe_mode
name: cam_hs.cam_hs (0x4800210c/0x0dc = 0x0118), b a24, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: cam_hs | ssi2_dat_tx | NA | NA | gpio_94 | hw_dbg0 | NA | safe_mode
name: cam_pclk.cam_pclk (0x48002112/0x0e2 = 0x0118), b c27, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: cam_pclk | NA | NA | NA | gpio_97 | hw_dbg2 | NA | safe_mode
name: cam_strobe.safe_mode (0x48002132/0x102 = 0x010f), b d25, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: cam_strobe | NA | NA | NA | gpio_126 | hw_dbg11 | NA | safe_mode
name: cam_vs.cam_vs (0x4800210e/0x0de = 0x0118), b a23, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: cam_vs | ssi2_flag_tx | NA | NA | gpio_95 | hw_dbg1 | NA | safe_mode
name: cam_wen.gpio_167 (0x48002130/0x100 = 0x010c), b b23, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4
signals: cam_wen | NA | cam_shutter | NA | gpio_167 | hw_dbg10 | NA | safe_mode
name: cam_xclka.cam_xclka (0x48002110/0x0e0 = 0x0000), b c25, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: cam_xclka | NA | NA | NA | gpio_96 | NA | NA | safe_mode
name: cam_xclkb.safe_mode (0x4800212e/0x0fe = 0x010f), b b26, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: cam_xclkb | NA | NA | NA | gpio_111 | NA | NA | safe_mode
name: csi2_dx0.safe_mode (0x48002134/0x104 = 0x010f), b ag19, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: csi2_dx0 | NA | NA | NA | gpio_112 | NA | NA | safe_mode
name: csi2_dx1.safe_mode (0x48002138/0x108 = 0x010f), b ag18, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: csi2_dx1 | NA | NA | NA | gpio_114 | NA | NA | safe_mode
name: csi2_dy0.safe_mode (0x48002136/0x106 = 0x010f), b ah19, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: csi2_dy0 | NA | NA | NA | gpio_113 | NA | NA | safe_mode
name: csi2_dy1.safe_mode (0x4800213a/0x10a = 0x010f), b ah18, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: csi2_dy1 | NA | NA | NA | gpio_115 | NA | NA | safe_mode
name: dss_acbias.dss_acbias (0x480020da/0x0aa = 0x0000), b e27, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: dss_acbias | NA | NA | NA | gpio_69 | NA | NA | safe_mode
name: dss_data0.safe_mode (0x480020dc/0x0ac = 0x010f), b ag22, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: dss_data0 | dsi_dx0 | uart1_cts | NA | gpio_70 | NA | NA | safe_mode
name: dss_data1.safe_mode (0x480020de/0x0ae = 0x010f), b ah22, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: dss_data1 | dsi_dy0 | uart1_rts | NA | gpio_71 | NA | NA | safe_mode
name: dss_data10.dss_data10 (0x480020f0/0x0c0 = 0x0000), b ad28, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: dss_data10 | NA | NA | NA | gpio_80 | NA | NA | safe_mode
name: dss_data11.dss_data11 (0x480020f2/0x0c2 = 0x0000), b ad27, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: dss_data11 | NA | NA | NA | gpio_81 | NA | NA | safe_mode
name: dss_data12.dss_data12 (0x480020f4/0x0c4 = 0x0000), b ab28, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: dss_data12 | NA | NA | NA | gpio_82 | NA | NA | safe_mode
name: dss_data13.dss_data13 (0x480020f6/0x0c6 = 0x0000), b ab27, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: dss_data13 | NA | NA | NA | gpio_83 | NA | NA | safe_mode
name: dss_data14.dss_data14 (0x480020f8/0x0c8 = 0x0000), b aa28, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: dss_data14 | NA | NA | NA | gpio_84 | NA | NA | safe_mode
name: dss_data15.dss_data15 (0x480020fa/0x0ca = 0x0000), b aa27, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: dss_data15 | NA | NA | NA | gpio_85 | NA | NA | safe_mode
name: dss_data16.dss_data16 (0x480020fc/0x0cc = 0x0000), b g25, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: dss_data16 | NA | NA | NA | gpio_86 | NA | NA | safe_mode
name: dss_data17.dss_data17 (0x480020fe/0x0ce = 0x0000), b h27, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: dss_data17 | NA | NA | NA | gpio_87 | NA | NA | safe_mode
name: dss_data18.dss_data0 (0x48002100/0x0d0 = 0x0003), b h26, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE3
signals: dss_data18 | NA | mcspi3_clk | dss_data0 | gpio_88 | NA | NA | safe_mode
name: dss_data19.dss_data1 (0x48002102/0x0d2 = 0x0003), b h25, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE3
signals: dss_data19 | NA | mcspi3_simo | dss_data1 | gpio_89 | NA | NA | safe_mode
name: dss_data2.safe_mode (0x480020e0/0x0b0 = 0x010f), b ag23, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: dss_data2 | dsi_dx1 | NA | NA | gpio_72 | NA | NA | safe_mode
name: dss_data20.dss_data2 (0x48002104/0x0d4 = 0x0003), b e28, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE3
signals: dss_data20 | NA | mcspi3_somi | dss_data2 | gpio_90 | NA | NA | safe_mode
name: dss_data21.dss_data3 (0x48002106/0x0d6 = 0x0003), b j26, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE3
signals: dss_data21 | NA | mcspi3_cs0 | dss_data3 | gpio_91 | NA | NA | safe_mode
name: dss_data22.dss_data4 (0x48002108/0x0d8 = 0x0003), b ac27, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE3
signals: dss_data22 | NA | mcspi3_cs1 | dss_data4 | gpio_92 | NA | NA | safe_mode
name: dss_data23.dss_data5 (0x4800210a/0x0da = 0x0003), b ac28, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE3
signals: dss_data23 | NA | NA | dss_data5 | gpio_93 | NA | NA | safe_mode
name: dss_data3.safe_mode (0x480020e2/0x0b2 = 0x010f), b ah23, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: dss_data3 | dsi_dy1 | NA | NA | gpio_73 | NA | NA | safe_mode
name: dss_data4.safe_mode (0x480020e4/0x0b4 = 0x010f), b ag24, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: dss_data4 | dsi_dx2 | uart3_rx_irrx | NA | gpio_74 | NA | NA | safe_mode
name: dss_data5.safe_mode (0x480020e6/0x0b6 = 0x010f), b ah24, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: dss_data5 | dsi_dy2 | uart3_tx_irtx | NA | gpio_75 | NA | NA | safe_mode
name: dss_data6.dss_data6 (0x480020e8/0x0b8 = 0x0000), b e26, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: dss_data6 | NA | uart1_tx | dssvenc656_data6 | gpio_76 | hw_dbg14 | NA | safe_mode
name: dss_data7.dss_data7 (0x480020ea/0x0ba = 0x0000), b f28, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: dss_data7 | NA | uart1_rx | dssvenc656_data7 | gpio_77 | hw_dbg15 | NA | safe_mode
name: dss_data8.dss_data8 (0x480020ec/0x0bc = 0x0000), b f27, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: dss_data8 | NA | uart3_rx_irrx | NA | gpio_78 | hw_dbg16 | NA | safe_mode
name: dss_data9.dss_data9 (0x480020ee/0x0be = 0x0000), b g26, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: dss_data9 | NA | uart3_tx_irtx | NA | gpio_79 | hw_dbg17 | NA | safe_mode
name: dss_hsync.dss_hsync (0x480020d6/0x0a6 = 0x0000), b d26, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: dss_hsync | NA | NA | NA | gpio_67 | hw_dbg13 | NA | safe_mode
name: dss_pclk.dss_pclk (0x480020d4/0x0a4 = 0x0000), b d28, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: dss_pclk | NA | NA | NA | gpio_66 | hw_dbg12 | NA | safe_mode
name: dss_vsync.dss_vsync (0x480020d8/0x0a8 = 0x0000), b d27, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: dss_vsync | NA | NA | NA | gpio_68 | NA | NA | safe_mode
name: etk_clk.hw_dbg0 (0x480025d8/0x5a8 = 0x010f), b af10, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: etk_clk | mcbsp5_clkx | sdmmc3_clk | hsusb1_stp | gpio_12 | mm1_rxdp | hsusb1_tll_stp | hw_dbg0
name: etk_ctl.hw_dbg1 (0x480025da/0x5aa = 0x010f), b ae10, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: etk_ctl | NA | sdmmc3_cmd | hsusb1_clk | gpio_13 | NA | hsusb1_tll_clk | hw_dbg1
name: etk_d0.hw_dbg2 (0x480025dc/0x5ac = 0x010f), b af11, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: etk_d0 | mcspi3_simo | sdmmc3_dat4 | hsusb1_data0 | gpio_14 | mm1_rxrcv | hsusb1_tll_data0 | hw_dbg2
name: etk_d1.hw_dbg3 (0x480025de/0x5ae = 0x010f), b ag12, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: etk_d1 | mcspi3_somi | NA | hsusb1_data1 | gpio_15 | mm1_txse0 | hsusb1_tll_data1 | hw_dbg3
name: etk_d10.hw_dbg12 (0x480025f0/0x5c0 = 0x010f), b ae7, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: etk_d10 | NA | uart1_rx | hsusb2_clk | gpio_24 | NA | hsusb2_tll_clk | hw_dbg12
name: etk_d11.gpio_25 (0x480025f2/0x5c2 = 0x0104), b af7, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE4
signals: etk_d11 | NA | NA | hsusb2_stp | gpio_25 | mm2_rxdp | hsusb2_tll_stp | hw_dbg13
name: etk_d12.gpio_26 (0x480025f4/0x5c4 = 0x010c), b ag7, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4
signals: etk_d12 | sys_drm_msecure | NA | hsusb2_dir | gpio_26 | NA | hsusb2_tll_dir | hw_dbg14
name: etk_d13.gpio_27 (0x480025f6/0x5c6 = 0x010c), b ah7, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4
signals: etk_d13 | NA | NA | hsusb2_nxt | gpio_27 | mm2_rxdm | hsusb2_tll_nxt | hw_dbg15
name: etk_d14.gpio_28 (0x480025f8/0x5c8 = 0x010c), b ag8, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4
signals: etk_d14 | NA | NA | hsusb2_data0 | gpio_28 | mm2_rxrcv | hsusb2_tll_data0 | hw_dbg16
name: etk_d15.gpio_29 (0x480025fa/0x5ca = 0x011c), b ah8, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE4
signals: etk_d15 | NA | NA | hsusb2_data1 | gpio_29 | mm2_txse0 | hsusb2_tll_data1 | hw_dbg17
name: etk_d2.hw_dbg4 (0x480025e0/0x5b0 = 0x010f), b ah12, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: etk_d2 | mcspi3_cs0 | NA | hsusb1_data2 | gpio_16 | mm1_txdat | hsusb1_tll_data2 | hw_dbg4
name: etk_d3.hw_dbg5 (0x480025e2/0x5b2 = 0x010f), b ae13, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: etk_d3 | mcspi3_clk | sdmmc3_dat3 | hsusb1_data7 | gpio_17 | NA | hsusb1_tll_data7 | hw_dbg5
name: etk_d4.hw_dbg6 (0x480025e4/0x5b4 = 0x010f), b ae11, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: etk_d4 | mcbsp5_dr | sdmmc3_dat0 | hsusb1_data4 | gpio_18 | NA | hsusb1_tll_data4 | hw_dbg6
name: etk_d5.hw_dbg7 (0x480025e6/0x5b6 = 0x010f), b ah9, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: etk_d5 | mcbsp5_fsx | sdmmc3_dat1 | hsusb1_data5 | gpio_19 | NA | hsusb1_tll_data5 | hw_dbg7
name: etk_d6.hw_dbg8 (0x480025e8/0x5b8 = 0x010f), b af13, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: etk_d6 | mcbsp5_dx | sdmmc3_dat2 | hsusb1_data6 | gpio_20 | NA | hsusb1_tll_data6 | hw_dbg8
name: etk_d7.hw_dbg9 (0x480025ea/0x5ba = 0x010f), b ah14, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: etk_d7 | mcspi3_cs1 | sdmmc3_dat7 | hsusb1_data3 | gpio_21 | mm1_txen_n | hsusb1_tll_data3 | hw_dbg9
name: etk_d8.hw_dbg10 (0x480025ec/0x5bc = 0x010f), b af9, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: etk_d8 | sys_drm_msecure | sdmmc3_dat6 | hsusb1_dir | gpio_22 | NA | hsusb1_tll_dir | hw_dbg10
name: etk_d9.hw_dbg11 (0x480025ee/0x5be = 0x010f), b ag9, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: etk_d9 | sys_secure_indicator | sdmmc3_dat5 | hsusb1_nxt | gpio_23 | mm1_rxdm | hsusb1_tll_nxt | hw_dbg11
name: gpmc_a1.gpio_34 (0x4800207a/0x04a = 0x010c), b n4, t ac15
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4
signals: gpmc_a1 | NA | NA | NA | gpio_34 | NA | NA | safe_mode
name: gpmc_a10.safe_mode (0x4800208c/0x05c = 0x010f), b k3, t ab19
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: gpmc_a10 | sys_ndmareq3 | NA | NA | gpio_43 | NA | NA | safe_mode
name: gpmc_a11.<NULL> (0x48002266/0x236 = 0x0109), b NA, t ac20
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE1
signals: gpmc_a11 | NA | NA | NA | NA | NA | NA | safe_mode
name: gpmc_a2.gpio_35 (0x4800207c/0x04c = 0x010c), b m4, t ab15
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4
signals: gpmc_a2 | NA | NA | NA | gpio_35 | NA | NA | safe_mode
name: gpmc_a3.gpio_36 (0x4800207e/0x04e = 0x000c), b l4, t ac16
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE4
signals: gpmc_a3 | NA | NA | NA | gpio_36 | NA | NA | safe_mode
name: gpmc_a4.gpio_37 (0x48002080/0x050 = 0x010c), b k4, t ab16
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4
signals: gpmc_a4 | NA | NA | NA | gpio_37 | NA | NA | safe_mode
name: gpmc_a5.gpio_38 (0x48002082/0x052 = 0x0004), b t3, t ac17
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE4
signals: gpmc_a5 | NA | NA | NA | gpio_38 | NA | NA | safe_mode
name: gpmc_a6.gpio_39 (0x48002084/0x054 = 0x0004), b r3, t ab17
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE4
signals: gpmc_a6 | NA | NA | NA | gpio_39 | NA | NA | safe_mode
name: gpmc_a7.safe_mode (0x48002086/0x056 = 0x010f), b n3, t ac18
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: gpmc_a7 | NA | NA | NA | gpio_40 | NA | NA | safe_mode
name: gpmc_a8.safe_mode (0x48002088/0x058 = 0x010f), b m3, t ab18
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: gpmc_a8 | NA | NA | NA | gpio_41 | NA | NA | safe_mode
name: gpmc_a9.safe_mode (0x4800208a/0x05a = 0x010f), b l3, t ac19
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: gpmc_a9 | sys_ndmareq2 | NA | NA | gpio_42 | NA | NA | safe_mode
name: gpmc_clk.safe_mode (0x480020be/0x08e = 0x010f), b t4, t w2
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: gpmc_clk | NA | NA | NA | gpio_59 | NA | NA | safe_mode
name: gpmc_d10.gpmc_d10 (0x480020a2/0x072 = 0x0100), b p1, t ab4
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: gpmc_d10 | NA | NA | NA | gpio_46 | NA | NA | safe_mode
name: gpmc_d11.gpmc_d11 (0x480020a4/0x074 = 0x0100), b r1, t ac4
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: gpmc_d11 | NA | NA | NA | gpio_47 | NA | NA | safe_mode
name: gpmc_d12.gpmc_d12 (0x480020a6/0x076 = 0x0100), b r2, t ab6
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: gpmc_d12 | NA | NA | NA | gpio_48 | NA | NA | safe_mode
name: gpmc_d13.gpmc_d13 (0x480020a8/0x078 = 0x0100), b t2, t ac6
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: gpmc_d13 | NA | NA | NA | gpio_49 | NA | NA | safe_mode
name: gpmc_d14.gpmc_d14 (0x480020aa/0x07a = 0x0100), b w1, t ab7
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: gpmc_d14 | NA | NA | NA | gpio_50 | NA | NA | safe_mode
name: gpmc_d15.gpmc_d15 (0x480020ac/0x07c = 0x0100), b y1, t ac7
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: gpmc_d15 | NA | NA | NA | gpio_51 | NA | NA | safe_mode
name: gpmc_d8.gpmc_d8 (0x4800209e/0x06e = 0x0100), b NA, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: gpmc_d8 | NA | NA | NA | gpio_44 | NA | NA | safe_mode
name: gpmc_d9.gpmc_d9 (0x480020a0/0x070 = 0x0100), b k2, t ac3
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: gpmc_d9 | NA | NA | NA | gpio_45 | NA | NA | safe_mode
name: gpmc_nbe0_cle.gpmc_nbe0_cle (0x480020c6/0x096 = 0x0000), b g3, t ac12
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: gpmc_nbe0_cle | NA | NA | NA | gpio_60 | NA | NA | safe_mode
name: gpmc_nbe1.safe_mode (0x480020c8/0x098 = 0x010f), b u3, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: gpmc_nbe1 | NA | NA | NA | gpio_61 | NA | NA | safe_mode
name: gpmc_ncs1.safe_mode (0x480020b0/0x080 = 0x010f), b h3, t y1
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: gpmc_ncs1 | NA | NA | NA | gpio_52 | NA | NA | safe_mode
name: gpmc_ncs2.gpio_53 (0x480020b2/0x082 = 0x011c), b v8, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE4
signals: gpmc_ncs2 | NA | NA | NA | gpio_53 | NA | NA | safe_mode
name: gpmc_ncs3.gpio_54 (0x480020b4/0x084 = 0x011c), b u8, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE4
signals: gpmc_ncs3 | sys_ndmareq0 | NA | NA | gpio_54 | NA | NA | safe_mode
name: gpmc_ncs4.gpio_55 (0x480020b6/0x086 = 0x0104), b t8, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE4
signals: gpmc_ncs4 | sys_ndmareq1 | mcbsp4_clkx | gpt9_pwm_evt | gpio_55 | NA | NA | safe_mode
name: gpmc_ncs5.gpio_56 (0x480020b8/0x088 = 0x0104), b r8, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE4
signals: gpmc_ncs5 | sys_ndmareq2 | mcbsp4_dr | gpt10_pwm_evt | gpio_56 | NA | NA | safe_mode
name: gpmc_ncs6.safe_mode (0x480020ba/0x08a = 0x010f), b p8, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: gpmc_ncs6 | sys_ndmareq3 | mcbsp4_dx | gpt11_pwm_evt | gpio_57 | NA | NA | safe_mode
name: gpmc_ncs7.safe_mode (0x480020bc/0x08c = 0x010f), b n8, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: gpmc_ncs7 | gpmc_io_dir | mcbsp4_fsx | gpt8_pwm_evt | gpio_58 | NA | NA | safe_mode
name: gpmc_nwp.gpmc_nwp (0x480020ca/0x09a = 0x0100), b h1, t ab10
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: gpmc_nwp | NA | NA | NA | gpio_62 | NA | NA | safe_mode
name: gpmc_wait1.safe_mode (0x480020ce/0x09e = 0x010f), b l8, t ac10
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: gpmc_wait1 | NA | NA | NA | gpio_63 | NA | NA | safe_mode
name: gpmc_wait2.safe_mode (0x480020d0/0x0a0 = 0x010f), b k8, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: gpmc_wait2 | NA | uart4_tx | NA | gpio_64 | NA | NA | safe_mode
name: gpmc_wait3.safe_mode (0x480020d2/0x0a2 = 0x010f), b j8, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: gpmc_wait3 | sys_ndmareq1 | uart4_rx | NA | gpio_65 | NA | NA | safe_mode
name: hdq_sio.safe_mode (0x480021c6/0x196 = 0x010f), b j25, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: hdq_sio | sys_altclk | i2c2_sccbe | i2c3_sccbe | gpio_170 | NA | NA | safe_mode
name: hsusb0_clk.hsusb0_clk (0x480021a2/0x172 = 0x0108), b t28, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0
signals: hsusb0_clk | NA | NA | NA | gpio_120 | NA | NA | safe_mode
name: hsusb0_data0.hsusb0_data0 (0x480021aa/0x17a = 0x0108), b t27, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0
signals: hsusb0_data0 | NA | uart3_tx_irtx | NA | gpio_125 | uart2_tx | NA | safe_mode
name: hsusb0_data1.hsusb0_data1 (0x480021ac/0x17c = 0x0108), b u28, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0
signals: hsusb0_data1 | NA | uart3_rx_irrx | NA | gpio_130 | uart2_rx | NA | safe_mode
name: hsusb0_data2.hsusb0_data2 (0x480021ae/0x17e = 0x0108), b u27, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0
signals: hsusb0_data2 | NA | uart3_rts_sd | NA | gpio_131 | uart2_rts | NA | safe_mode
name: hsusb0_data3.hsusb0_data3 (0x480021b0/0x180 = 0x0108), b u26, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0
signals: hsusb0_data3 | NA | uart3_cts_rctx | NA | gpio_169 | uart2_cts | NA | safe_mode
name: hsusb0_data4.hsusb0_data4 (0x480021b2/0x182 = 0x0108), b u25, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0
signals: hsusb0_data4 | NA | NA | NA | gpio_188 | NA | NA | safe_mode
name: hsusb0_data5.hsusb0_data5 (0x480021b4/0x184 = 0x0108), b v28, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0
signals: hsusb0_data5 | NA | NA | NA | gpio_189 | NA | NA | safe_mode
name: hsusb0_data6.hsusb0_data6 (0x480021b6/0x186 = 0x0108), b v27, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0
signals: hsusb0_data6 | NA | NA | NA | gpio_190 | NA | NA | safe_mode
name: hsusb0_data7.hsusb0_data7 (0x480021b8/0x188 = 0x0108), b v26, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0
signals: hsusb0_data7 | NA | NA | NA | gpio_191 | NA | NA | safe_mode
name: hsusb0_dir.hsusb0_dir (0x480021a6/0x176 = 0x0108), b r28, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0
signals: hsusb0_dir | NA | NA | NA | gpio_122 | NA | NA | safe_mode
name: hsusb0_nxt.hsusb0_nxt (0x480021a8/0x178 = 0x0108), b t26, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0
signals: hsusb0_nxt | NA | NA | NA | gpio_124 | NA | NA | safe_mode
name: hsusb0_stp.hsusb0_stp (0x480021a4/0x174 = 0x0108), b t25, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0
signals: hsusb0_stp | NA | NA | NA | gpio_121 | NA | NA | safe_mode
name: i2c2_scl.i2c2_scl (0x480021be/0x18e = 0x0100), b af15, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: i2c2_scl | NA | NA | NA | gpio_168 | NA | NA | safe_mode
name: i2c2_sda.i2c2_sda (0x480021c0/0x190 = 0x0100), b ae15, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: i2c2_sda | NA | NA | NA | gpio_183 | NA | NA | safe_mode
name: i2c3_scl.i2c3_scl (0x480021c2/0x192 = 0x0100), b af14, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: i2c3_scl | NA | NA | NA | gpio_184 | NA | NA | safe_mode
name: i2c3_sda.i2c3_sda (0x480021c4/0x194 = 0x0100), b ag14, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: i2c3_sda | NA | NA | NA | gpio_185 | NA | NA | safe_mode
name: i2c4_scl.i2c4_scl (0x48002a00/0x9d0 = 0x0118), b ad26, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: i2c4_scl | sys_nvmode1 | NA | NA | NA | NA | NA | safe_mode
name: i2c4_sda.i2c4_sda (0x48002a02/0x9d2 = 0x0118), b ae26, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: i2c4_sda | sys_nvmode2 | NA | NA | NA | NA | NA | safe_mode
name: jtag_emu0.jtag_emu0 (0x48002a24/0x9f4 = 0x0100), b aa11, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: jtag_emu0 | NA | NA | NA | gpio_11 | NA | NA | safe_mode
name: jtag_emu1.jtag_emu1 (0x48002a26/0x9f6 = 0x0100), b aa10, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: jtag_emu1 | NA | NA | NA | gpio_31 | NA | NA | safe_mode
name: mcbsp1_clkr.gpio_156 (0x4800218c/0x15c = 0x0004), b y21, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE4
signals: mcbsp1_clkr | mcspi4_clk | sim_cd | NA | gpio_156 | NA | NA | safe_mode
name: mcbsp1_clkx.gpio_162 (0x48002198/0x168 = 0x0004), b w21, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE4
signals: mcbsp1_clkx | NA | mcbsp3_clkx | NA | gpio_162 | NA | NA | safe_mode
name: mcbsp1_dr.gpio_159 (0x48002192/0x162 = 0x011c), b u21, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE4
signals: mcbsp1_dr | mcspi4_somi | mcbsp3_dr | NA | gpio_159 | NA | NA | safe_mode
name: mcbsp1_dx.gpio_158 (0x48002190/0x160 = 0x010c), b v21, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4
signals: mcbsp1_dx | mcspi4_simo | mcbsp3_dx | NA | gpio_158 | NA | NA | safe_mode
name: mcbsp1_fsr.gpio_157 (0x4800218e/0x15e = 0x0004), b aa21, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE4
signals: mcbsp1_fsr | adpllv2d_dithering_en1 | cam_global_reset | NA | gpio_157 | NA | NA | safe_mode
name: mcbsp1_fsx.gpio_161 (0x48002196/0x166 = 0x000c), b k26, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE4
signals: mcbsp1_fsx | mcspi4_cs0 | mcbsp3_fsx | NA | gpio_161 | NA | NA | safe_mode
name: mcbsp2_clkx.mcbsp2_clkx (0x4800213e/0x10e = 0x0100), b n21, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: mcbsp2_clkx | NA | NA | NA | gpio_117 | NA | NA | safe_mode
name: mcbsp2_dr.mcbsp2_dr (0x48002140/0x110 = 0x0100), b r21, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: mcbsp2_dr | NA | NA | NA | gpio_118 | NA | NA | safe_mode
name: mcbsp2_dx.mcbsp2_dx (0x48002142/0x112 = 0x0000), b m21, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: mcbsp2_dx | NA | NA | NA | gpio_119 | NA | NA | safe_mode
name: mcbsp2_fsx.mcbsp2_fsx (0x4800213c/0x10c = 0x0100), b p21, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: mcbsp2_fsx | NA | NA | NA | gpio_116 | NA | NA | safe_mode
name: mcbsp3_clkx.safe_mode (0x48002170/0x140 = 0x010f), b af5, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: mcbsp3_clkx | uart2_tx | NA | NA | gpio_142 | hsusb3_tll_data6 | NA | safe_mode
name: mcbsp3_dr.safe_mode (0x4800216e/0x13e = 0x010f), b ae6, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: mcbsp3_dr | uart2_rts | NA | NA | gpio_141 | hsusb3_tll_data5 | NA | safe_mode
name: mcbsp3_dx.safe_mode (0x4800216c/0x13c = 0x010f), b af6, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: mcbsp3_dx | uart2_cts | NA | NA | gpio_140 | hsusb3_tll_data4 | NA | safe_mode
name: mcbsp3_fsx.safe_mode (0x48002172/0x142 = 0x010f), b ae5, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: mcbsp3_fsx | uart2_rx | NA | NA | gpio_143 | hsusb3_tll_data7 | NA | safe_mode
name: mcbsp4_clkx.gpio_152 (0x48002184/0x154 = 0x0104), b ae1, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE4
signals: mcbsp4_clkx | ssi1_dat_rx | NA | NA | gpio_152 | hsusb3_tll_data1 | mm3_txse0 | safe_mode
name: mcbsp4_dr.gpio_153 (0x48002186/0x156 = 0x010c), b ad1, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4
signals: mcbsp4_dr | ssi1_flag_rx | NA | NA | gpio_153 | hsusb3_tll_data0 | mm3_rxrcv | safe_mode
name: mcbsp4_dx.gpio_154 (0x48002188/0x158 = 0x010c), b ad2, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4
signals: mcbsp4_dx | ssi1_rdy_rx | NA | NA | gpio_154 | hsusb3_tll_data2 | mm3_txdat | safe_mode
name: mcbsp4_fsx.gpio_155 (0x4800218a/0x15a = 0x0004), b ac1, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE4
signals: mcbsp4_fsx | ssi1_wake | NA | NA | gpio_155 | hsusb3_tll_data3 | mm3_txen_n | safe_mode
name: mcbsp_clks.mcbsp_clks (0x48002194/0x164 = 0x0110), b t21, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: mcbsp_clks | NA | cam_shutter | NA | gpio_160 | uart1_cts | NA | safe_mode
name: mcspi1_clk.sdmmc2_dat4 (0x480021c8/0x198 = 0x0119), b ab3, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE1
signals: mcspi1_clk | sdmmc2_dat4 | NA | NA | gpio_171 | NA | NA | safe_mode
name: mcspi1_cs0.sdmmc2_dat7 (0x480021ce/0x19e = 0x0119), b ac2, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE1
signals: mcspi1_cs0 | sdmmc2_dat7 | NA | NA | gpio_174 | NA | NA | safe_mode
name: mcspi1_cs1.sdmmc3_cmd (0x480021d0/0x1a0 = 0x011b), b ac3, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE3
signals: mcspi1_cs1 | adpllv2d_dithering_en2 | NA | sdmmc3_cmd | gpio_175 | NA | NA | safe_mode
name: mcspi1_cs2.sdmmc3_clk (0x480021d2/0x1a2 = 0x011b), b ab1, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE3
signals: mcspi1_cs2 | NA | NA | sdmmc3_clk | gpio_176 | NA | NA | safe_mode
name: mcspi1_cs3.gpio_177 (0x480021d4/0x1a4 = 0x010c), b ab2, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4
signals: mcspi1_cs3 | NA | hsusb2_tll_data2 | hsusb2_data2 | gpio_177 | mm2_txdat | NA | safe_mode
name: mcspi1_simo.sdmmc2_dat5 (0x480021ca/0x19a = 0x0119), b ab4, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE1
signals: mcspi1_simo | sdmmc2_dat5 | NA | NA | gpio_172 | NA | NA | safe_mode
name: mcspi1_somi.sdmmc2_dat6 (0x480021cc/0x19c = 0x0119), b aa4, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE1
signals: mcspi1_somi | sdmmc2_dat6 | NA | NA | gpio_173 | NA | NA | safe_mode
name: mcspi2_clk.mcspi2_clk (0x480021d6/0x1a6 = 0x0100), b aa3, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: mcspi2_clk | NA | hsusb2_tll_data7 | hsusb2_data7 | gpio_178 | NA | NA | safe_mode
name: mcspi2_cs0.gpio_181 (0x480021dc/0x1ac = 0x010c), b y4, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4
signals: mcspi2_cs0 | gpt11_pwm_evt | hsusb2_tll_data6 | hsusb2_data6 | gpio_181 | NA | NA | safe_mode
name: mcspi2_cs1.gpio_182 (0x480021de/0x1ae = 0x010c), b v3, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE4
signals: mcspi2_cs1 | gpt8_pwm_evt | hsusb2_tll_data3 | hsusb2_data3 | gpio_182 | mm2_txen_n | NA | safe_mode
name: mcspi2_simo.mcspi2_simo (0x480021d8/0x1a8 = 0x0100), b y2, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: mcspi2_simo | gpt9_pwm_evt | hsusb2_tll_data4 | hsusb2_data4 | gpio_179 | NA | NA | safe_mode
name: mcspi2_somi.mcspi2_somi (0x480021da/0x1aa = 0x0100), b y3, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: mcspi2_somi | gpt10_pwm_evt | hsusb2_tll_data5 | hsusb2_data5 | gpio_180 | NA | NA | safe_mode
name: sad2d_mbusflag.sad2d_mbusflag (0x4800225e/0x22e = 0x0100), b NA, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: sad2d_mbusflag | mad2d_sbusflag | NA | NA | NA | NA | NA | NA
name: sad2d_mcad28.sad2d_mcad28 (0x4800221c/0x1ec = 0x0108), b NA, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0
signals: sad2d_mcad28 | mad2d_mcad28 | NA | NA | NA | NA | NA | NA
name: sad2d_mcad29.sad2d_mcad29 (0x4800221e/0x1ee = 0x0108), b NA, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0
signals: sad2d_mcad29 | mad2d_mcad29 | NA | NA | NA | NA | NA | NA
name: sad2d_mcad32.sad2d_mcad32 (0x48002224/0x1f4 = 0x0108), b NA, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0
signals: sad2d_mcad32 | mad2d_mcad32 | NA | NA | NA | NA | NA | NA
name: sad2d_mcad33.sad2d_mcad33 (0x48002226/0x1f6 = 0x0108), b NA, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0
signals: sad2d_mcad33 | mad2d_mcad33 | NA | NA | NA | NA | NA | NA
name: sad2d_mcad34.sad2d_mcad34 (0x48002228/0x1f8 = 0x0108), b NA, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0
signals: sad2d_mcad34 | mad2d_mcad34 | NA | NA | NA | NA | NA | NA
name: sad2d_mcad35.sad2d_mcad35 (0x4800222a/0x1fa = 0x0108), b NA, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0
signals: sad2d_mcad35 | mad2d_mcad35 | NA | NA | NA | NA | NA | NA
name: sad2d_mcad36.sad2d_mcad36 (0x4800222c/0x1fc = 0x0108), b NA, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE0
signals: sad2d_mcad36 | mad2d_mcad36 | NA | NA | NA | NA | NA | NA
name: sad2d_mread.sad2d_mread (0x4800225a/0x22a = 0x0100), b NA, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: sad2d_mread | mad2d_sread | NA | NA | NA | NA | NA | NA
name: sad2d_mwrite.sad2d_mwrite (0x48002256/0x226 = 0x0100), b NA, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: sad2d_mwrite | mad2d_swrite | NA | NA | NA | NA | NA | NA
name: sad2d_sbusflag.sad2d_sbusflag (0x48002260/0x230 = 0x0100), b NA, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: sad2d_sbusflag | mad2d_mbusflag | NA | NA | NA | NA | NA | NA
name: sad2d_sread.sad2d_sread (0x4800225c/0x22c = 0x0100), b NA, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: sad2d_sread | mad2d_mread | NA | NA | NA | NA | NA | NA
name: sad2d_swrite.sad2d_swrite (0x48002258/0x228 = 0x0100), b NA, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: sad2d_swrite | mad2d_mwrite | NA | NA | NA | NA | NA | NA
name: sdmmc1_clk.sdmmc1_clk (0x48002144/0x114 = 0x0118), b n28, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: sdmmc1_clk | ms_clk | NA | NA | gpio_120 | NA | NA | safe_mode
name: sdmmc1_cmd.sdmmc1_cmd (0x48002146/0x116 = 0x0118), b m27, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: sdmmc1_cmd | ms_bs | NA | NA | gpio_121 | NA | NA | safe_mode
name: sdmmc1_dat0.sdmmc1_dat0 (0x48002148/0x118 = 0x0118), b n27, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: sdmmc1_dat0 | ms_dat0 | NA | NA | gpio_122 | NA | NA | safe_mode
name: sdmmc1_dat1.sdmmc1_dat1 (0x4800214a/0x11a = 0x0118), b n26, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: sdmmc1_dat1 | ms_dat1 | NA | NA | gpio_123 | NA | NA | safe_mode
name: sdmmc1_dat2.sdmmc1_dat2 (0x4800214c/0x11c = 0x0118), b n25, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: sdmmc1_dat2 | ms_dat2 | NA | NA | gpio_124 | NA | NA | safe_mode
name: sdmmc1_dat3.sdmmc1_dat3 (0x4800214e/0x11e = 0x0118), b p28, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: sdmmc1_dat3 | ms_dat3 | NA | NA | gpio_125 | NA | NA | safe_mode
name: sdmmc2_clk.sdmmc2_clk (0x48002158/0x128 = 0x0118), b ae2, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: sdmmc2_clk | mcspi3_clk | NA | NA | gpio_130 | NA | NA | safe_mode
name: sdmmc2_cmd.sdmmc2_cmd (0x4800215a/0x12a = 0x0118), b ag5, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: sdmmc2_cmd | mcspi3_simo | NA | NA | gpio_131 | NA | NA | safe_mode
name: sdmmc2_dat0.sdmmc2_dat0 (0x4800215c/0x12c = 0x0118), b ah5, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: sdmmc2_dat0 | mcspi3_somi | NA | NA | gpio_132 | NA | NA | safe_mode
name: sdmmc2_dat1.sdmmc2_dat1 (0x4800215e/0x12e = 0x0118), b ah4, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: sdmmc2_dat1 | NA | NA | NA | gpio_133 | NA | NA | safe_mode
name: sdmmc2_dat2.sdmmc2_dat2 (0x48002160/0x130 = 0x0118), b ag4, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: sdmmc2_dat2 | mcspi3_cs1 | NA | NA | gpio_134 | NA | NA | safe_mode
name: sdmmc2_dat3.sdmmc2_dat3 (0x48002162/0x132 = 0x0118), b af4, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: sdmmc2_dat3 | mcspi3_cs0 | NA | NA | gpio_135 | NA | NA | safe_mode
name: sdmmc2_dat4.sdmmc3_dat0 (0x48002164/0x134 = 0x011b), b ae4, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE3
signals: sdmmc2_dat4 | sdmmc2_dir_dat0 | NA | sdmmc3_dat0 | gpio_136 | NA | NA | safe_mode
name: sdmmc2_dat5.sdmmc3_dat1 (0x48002166/0x136 = 0x011b), b ah3, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE3
signals: sdmmc2_dat5 | sdmmc2_dir_dat1 | cam_global_reset | sdmmc3_dat1 | gpio_137 | hsusb3_tll_stp | mm3_rxdp | safe_mode
name: sdmmc2_dat6.sdmmc3_dat2 (0x48002168/0x138 = 0x011b), b af3, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE3
signals: sdmmc2_dat6 | sdmmc2_dir_cmd | cam_shutter | sdmmc3_dat2 | gpio_138 | hsusb3_tll_dir | NA | safe_mode
name: sdmmc2_dat7.sdmmc3_dat3 (0x4800216a/0x13a = 0x011b), b ae3, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE3
signals: sdmmc2_dat7 | sdmmc2_clkin | NA | sdmmc3_dat3 | gpio_139 | hsusb3_tll_nxt | mm3_rxdm | safe_mode
name: sdrc_cke0.sdrc_cke0 (0x48002262/0x232 = 0x0118), b h16, t j22
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: sdrc_cke0 | NA | NA | NA | NA | NA | NA | safe_mode_out1
name: sdrc_cke1.sdrc_cke1 (0x48002264/0x234 = 0x0118), b h17, t j23
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: sdrc_cke1 | NA | NA | NA | NA | NA | NA | safe_mode_out1
name: sim_clk.sim_clk (0x48002152/0x122 = 0x0100), b p26, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: sim_clk | NA | NA | NA | gpio_127 | NA | NA | safe_mode
name: sim_io.sim_io (0x48002150/0x120 = 0x0100), b p27, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: sim_io | sim_io_low_impedance | NA | NA | gpio_126 | NA | NA | safe_mode
name: sim_pwrctrl.sim_pwrctrl (0x48002154/0x124 = 0x0100), b r27, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: sim_pwrctrl | NA | NA | NA | gpio_128 | NA | NA | safe_mode
name: sim_rst.sim_rst (0x48002156/0x126 = 0x0100), b r25, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: sim_rst | NA | NA | NA | gpio_129 | NA | NA | safe_mode
name: sys_boot0.safe_mode (0x48002a0a/0x9da = 0x0107), b ah26, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE7
signals: sys_boot0 | NA | NA | dss_data18 | gpio_2 | NA | NA | safe_mode
name: sys_boot1.safe_mode (0x48002a0c/0x9dc = 0x0107), b ag26, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE7
signals: sys_boot1 | NA | NA | dss_data19 | gpio_3 | NA | NA | safe_mode
name: sys_boot2.safe_mode (0x48002a0e/0x9de = 0x0107), b ae14, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE7
signals: sys_boot2 | NA | NA | NA | gpio_4 | NA | NA | safe_mode
name: sys_boot3.safe_mode (0x48002a10/0x9e0 = 0x0107), b af18, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE7
signals: sys_boot3 | NA | NA | dss_data20 | gpio_5 | NA | NA | safe_mode
name: sys_boot4.safe_mode (0x48002a12/0x9e2 = 0x0107), b af19, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE7
signals: sys_boot4 | sdmmc2_dir_dat2 | NA | dss_data21 | gpio_6 | NA | NA | safe_mode
name: sys_boot5.safe_mode (0x48002a14/0x9e4 = 0x0107), b ae21, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE7
signals: sys_boot5 | sdmmc2_dir_dat3 | NA | dss_data22 | gpio_7 | NA | NA | safe_mode
name: sys_boot6.safe_mode (0x48002a16/0x9e6 = 0x0107), b af21, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE7
signals: sys_boot6 | NA | NA | dss_data23 | gpio_8 | NA | NA | safe_mode
name: sys_clkout1.safe_mode (0x48002a1a/0x9ea = 0x010f), b ag25, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: sys_clkout1 | NA | NA | NA | gpio_10 | NA | NA | safe_mode
name: sys_clkout2.gpio_186 (0x480021e2/0x1b2 = 0x0004), b ae22, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE4
signals: sys_clkout2 | NA | NA | NA | gpio_186 | NA | NA | safe_mode
name: sys_clkreq.sys_clkreq (0x48002a06/0x9d6 = 0x0100), b af25, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: sys_clkreq | NA | NA | NA | gpio_1 | NA | NA | safe_mode
name: sys_nirq.sys_nirq (0x480021e0/0x1b0 = 0x4118), b af26, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_PIN_OFF_WAKEUPENABLE | OMAP_MUX_MODE0
signals: sys_nirq | NA | NA | NA | gpio_0 | NA | NA | safe_mode
name: sys_nreswarm.sys_nreswarm (0x48002a08/0x9d8 = 0x0118), b af24, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: sys_nreswarm | NA | NA | NA | gpio_30 | NA | NA | safe_mode
name: sys_off_mode.sys_off_mode (0x48002a18/0x9e8 = 0x0100), b af22, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: sys_off_mode | NA | NA | NA | gpio_9 | NA | NA | safe_mode
name: uart1_cts.uart1_cts (0x48002180/0x150 = 0x0118), b w8, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: uart1_cts | ssi1_rdy_tx | NA | NA | gpio_150 | hsusb3_tll_clk | NA | safe_mode
name: uart1_rts.uart1_rts (0x4800217e/0x14e = 0x0000), b aa9, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: uart1_rts | ssi1_flag_tx | NA | NA | gpio_149 | NA | NA | safe_mode
name: uart1_rx.uart1_rx (0x48002182/0x152 = 0x0100), b y8, t NA
mode: OMAP_PIN_INPUT | OMAP_MUX_MODE0
signals: uart1_rx | NA | mcbsp1_clkr | mcspi4_clk | gpio_151 | NA | NA | safe_mode
name: uart1_tx.uart1_tx (0x4800217c/0x14c = 0x0000), b aa8, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: uart1_tx | ssi1_dat_tx | NA | NA | gpio_148 | NA | NA | safe_mode
name: uart2_cts.safe_mode (0x48002174/0x144 = 0x010f), b ab26, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: uart2_cts | mcbsp3_dx | gpt9_pwm_evt | NA | gpio_144 | NA | NA | safe_mode
name: uart2_rts.safe_mode (0x48002176/0x146 = 0x010f), b ab25, t NA
mode: OMAP_PIN_INPUT_PULLDOWN | OMAP_MUX_MODE7
signals: uart2_rts | mcbsp3_dr | gpt10_pwm_evt | NA | gpio_145 | NA | NA | safe_mode
name: uart2_rx.uart2_rx (0x4800217a/0x14a = 0x0118), b ad25, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: uart2_rx | mcbsp3_fsx | gpt8_pwm_evt | NA | gpio_147 | NA | NA | safe_mode
name: uart2_tx.uart2_tx (0x48002178/0x148 = 0x0000), b aa25, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: uart2_tx | mcbsp3_clkx | gpt11_pwm_evt | NA | gpio_146 | NA | NA | safe_mode
name: uart3_cts_rctx.gpio_163 (0x4800219a/0x16a = 0x011c), b h18, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE4
signals: uart3_cts_rctx | NA | NA | NA | gpio_163 | NA | NA | safe_mode
name: uart3_rts_sd.gpio_164 (0x4800219c/0x16c = 0x0004), b h19, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE4
signals: uart3_rts_sd | NA | NA | NA | gpio_164 | NA | NA | safe_mode
name: uart3_rx_irrx.uart3_rx_irrx (0x4800219e/0x16e = 0x0118), b h20, t NA
mode: OMAP_PIN_INPUT_PULLUP | OMAP_MUX_MODE0
signals: uart3_rx_irrx | NA | NA | NA | gpio_165 | NA | NA | safe_mode
name: uart3_tx_irtx.uart3_tx_irtx (0x480021a0/0x170 = 0x0000), b h21, t NA
mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE0
signals: uart3_tx_irtx | NA | NA | NA | gpio_166 | NA | NA | safe_mode

/sys/kernel/debug/omap_mux/board
/sys/kernel/debug/omap_mux/cam_d0
/sys/kernel/debug/omap_mux/cam_d1
/sys/kernel/debug/omap_mux/cam_d10
/sys/kernel/debug/omap_mux/cam_d11
/sys/kernel/debug/omap_mux/cam_d2
/sys/kernel/debug/omap_mux/cam_d3
/sys/kernel/debug/omap_mux/cam_d4
/sys/kernel/debug/omap_mux/cam_d5
/sys/kernel/debug/omap_mux/cam_d6
/sys/kernel/debug/omap_mux/cam_d7
/sys/kernel/debug/omap_mux/cam_d8
/sys/kernel/debug/omap_mux/cam_d9
/sys/kernel/debug/omap_mux/cam_fld
/sys/kernel/debug/omap_mux/cam_hs
/sys/kernel/debug/omap_mux/cam_pclk
/sys/kernel/debug/omap_mux/cam_strobe
/sys/kernel/debug/omap_mux/cam_vs
/sys/kernel/debug/omap_mux/cam_wen
/sys/kernel/debug/omap_mux/cam_xclka
/sys/kernel/debug/omap_mux/cam_xclkb
/sys/kernel/debug/omap_mux/csi2_dx0
/sys/kernel/debug/omap_mux/csi2_dx1
/sys/kernel/debug/omap_mux/csi2_dy0
/sys/kernel/debug/omap_mux/csi2_dy1
/sys/kernel/debug/omap_mux/dss_acbias
/sys/kernel/debug/omap_mux/dss_data0
/sys/kernel/debug/omap_mux/dss_data1
/sys/kernel/debug/omap_mux/dss_data10
/sys/kernel/debug/omap_mux/dss_data11
/sys/kernel/debug/omap_mux/dss_data12
/sys/kernel/debug/omap_mux/dss_data13
/sys/kernel/debug/omap_mux/dss_data14
/sys/kernel/debug/omap_mux/dss_data15
/sys/kernel/debug/omap_mux/dss_data16
/sys/kernel/debug/omap_mux/dss_data17
/sys/kernel/debug/omap_mux/dss_data18
/sys/kernel/debug/omap_mux/dss_data19
/sys/kernel/debug/omap_mux/dss_data2
/sys/kernel/debug/omap_mux/dss_data20
/sys/kernel/debug/omap_mux/dss_data21
/sys/kernel/debug/omap_mux/dss_data22
/sys/kernel/debug/omap_mux/dss_data23
/sys/kernel/debug/omap_mux/dss_data3
/sys/kernel/debug/omap_mux/dss_data4
/sys/kernel/debug/omap_mux/dss_data5
/sys/kernel/debug/omap_mux/dss_data6
/sys/kernel/debug/omap_mux/dss_data7
/sys/kernel/debug/omap_mux/dss_data8
/sys/kernel/debug/omap_mux/dss_data9
/sys/kernel/debug/omap_mux/dss_hsync
/sys/kernel/debug/omap_mux/dss_pclk
/sys/kernel/debug/omap_mux/dss_vsync
/sys/kernel/debug/omap_mux/etk_clk
/sys/kernel/debug/omap_mux/etk_ctl
/sys/kernel/debug/omap_mux/etk_d0
/sys/kernel/debug/omap_mux/etk_d1
/sys/kernel/debug/omap_mux/etk_d10
/sys/kernel/debug/omap_mux/etk_d11
/sys/kernel/debug/omap_mux/etk_d12
/sys/kernel/debug/omap_mux/etk_d13
/sys/kernel/debug/omap_mux/etk_d14
/sys/kernel/debug/omap_mux/etk_d15
/sys/kernel/debug/omap_mux/etk_d2
/sys/kernel/debug/omap_mux/etk_d3
/sys/kernel/debug/omap_mux/etk_d4
/sys/kernel/debug/omap_mux/etk_d5
/sys/kernel/debug/omap_mux/etk_d6
/sys/kernel/debug/omap_mux/etk_d7
/sys/kernel/debug/omap_mux/etk_d8
/sys/kernel/debug/omap_mux/etk_d9
/sys/kernel/debug/omap_mux/gpmc_a1
/sys/kernel/debug/omap_mux/gpmc_a10
/sys/kernel/debug/omap_mux/gpmc_a11
/sys/kernel/debug/omap_mux/gpmc_a2
/sys/kernel/debug/omap_mux/gpmc_a3
/sys/kernel/debug/omap_mux/gpmc_a4
/sys/kernel/debug/omap_mux/gpmc_a5
/sys/kernel/debug/omap_mux/gpmc_a6
/sys/kernel/debug/omap_mux/gpmc_a7
/sys/kernel/debug/omap_mux/gpmc_a8
/sys/kernel/debug/omap_mux/gpmc_a9
/sys/kernel/debug/omap_mux/gpmc_clk
/sys/kernel/debug/omap_mux/gpmc_d10
/sys/kernel/debug/omap_mux/gpmc_d11
/sys/kernel/debug/omap_mux/gpmc_d12
/sys/kernel/debug/omap_mux/gpmc_d13
/sys/kernel/debug/omap_mux/gpmc_d14
/sys/kernel/debug/omap_mux/gpmc_d15
/sys/kernel/debug/omap_mux/gpmc_d8
/sys/kernel/debug/omap_mux/gpmc_d9
/sys/kernel/debug/omap_mux/gpmc_nbe0_cle
/sys/kernel/debug/omap_mux/gpmc_nbe1
/sys/kernel/debug/omap_mux/gpmc_ncs1
/sys/kernel/debug/omap_mux/gpmc_ncs2
/sys/kernel/debug/omap_mux/gpmc_ncs3
/sys/kernel/debug/omap_mux/gpmc_ncs4
/sys/kernel/debug/omap_mux/gpmc_ncs5
/sys/kernel/debug/omap_mux/gpmc_ncs6
/sys/kernel/debug/omap_mux/gpmc_ncs7
/sys/kernel/debug/omap_mux/gpmc_nwp
/sys/kernel/debug/omap_mux/gpmc_wait1
/sys/kernel/debug/omap_mux/gpmc_wait2
/sys/kernel/debug/omap_mux/gpmc_wait3
/sys/kernel/debug/omap_mux/hdq_sio
/sys/kernel/debug/omap_mux/hsusb0_clk
/sys/kernel/debug/omap_mux/hsusb0_data0
/sys/kernel/debug/omap_mux/hsusb0_data1
/sys/kernel/debug/omap_mux/hsusb0_data2
/sys/kernel/debug/omap_mux/hsusb0_data3
/sys/kernel/debug/omap_mux/hsusb0_data4
/sys/kernel/debug/omap_mux/hsusb0_data5
/sys/kernel/debug/omap_mux/hsusb0_data6
/sys/kernel/debug/omap_mux/hsusb0_data7
/sys/kernel/debug/omap_mux/hsusb0_dir
/sys/kernel/debug/omap_mux/hsusb0_nxt
/sys/kernel/debug/omap_mux/hsusb0_stp
/sys/kernel/debug/omap_mux/i2c2_scl
/sys/kernel/debug/omap_mux/i2c2_sda
/sys/kernel/debug/omap_mux/i2c3_scl
/sys/kernel/debug/omap_mux/i2c3_sda
/sys/kernel/debug/omap_mux/i2c4_scl
/sys/kernel/debug/omap_mux/i2c4_sda
/sys/kernel/debug/omap_mux/jtag_emu0
/sys/kernel/debug/omap_mux/jtag_emu1
/sys/kernel/debug/omap_mux/mcbsp1_clkr
/sys/kernel/debug/omap_mux/mcbsp1_clkx
/sys/kernel/debug/omap_mux/mcbsp1_dr
/sys/kernel/debug/omap_mux/mcbsp1_dx
/sys/kernel/debug/omap_mux/mcbsp1_fsr
/sys/kernel/debug/omap_mux/mcbsp1_fsx
/sys/kernel/debug/omap_mux/mcbsp2_clkx
/sys/kernel/debug/omap_mux/mcbsp2_dr
/sys/kernel/debug/omap_mux/mcbsp2_dx
/sys/kernel/debug/omap_mux/mcbsp2_fsx
/sys/kernel/debug/omap_mux/mcbsp3_clkx
/sys/kernel/debug/omap_mux/mcbsp3_dr
/sys/kernel/debug/omap_mux/mcbsp3_dx
/sys/kernel/debug/omap_mux/mcbsp3_fsx
/sys/kernel/debug/omap_mux/mcbsp4_clkx
/sys/kernel/debug/omap_mux/mcbsp4_dr
/sys/kernel/debug/omap_mux/mcbsp4_dx
/sys/kernel/debug/omap_mux/mcbsp4_fsx
/sys/kernel/debug/omap_mux/mcbsp_clks
/sys/kernel/debug/omap_mux/mcspi1_clk
/sys/kernel/debug/omap_mux/mcspi1_cs0
/sys/kernel/debug/omap_mux/mcspi1_cs1
/sys/kernel/debug/omap_mux/mcspi1_cs2
/sys/kernel/debug/omap_mux/mcspi1_cs3
/sys/kernel/debug/omap_mux/mcspi1_simo
/sys/kernel/debug/omap_mux/mcspi1_somi
/sys/kernel/debug/omap_mux/mcspi2_clk
/sys/kernel/debug/omap_mux/mcspi2_cs0
/sys/kernel/debug/omap_mux/mcspi2_cs1
/sys/kernel/debug/omap_mux/mcspi2_simo
/sys/kernel/debug/omap_mux/mcspi2_somi
/sys/kernel/debug/omap_mux/sad2d_mbusflag
/sys/kernel/debug/omap_mux/sad2d_mcad28
/sys/kernel/debug/omap_mux/sad2d_mcad29
/sys/kernel/debug/omap_mux/sad2d_mcad32
/sys/kernel/debug/omap_mux/sad2d_mcad33
/sys/kernel/debug/omap_mux/sad2d_mcad34
/sys/kernel/debug/omap_mux/sad2d_mcad35
/sys/kernel/debug/omap_mux/sad2d_mcad36
/sys/kernel/debug/omap_mux/sad2d_mread
/sys/kernel/debug/omap_mux/sad2d_mwrite
/sys/kernel/debug/omap_mux/sad2d_sbusflag
/sys/kernel/debug/omap_mux/sad2d_sread
/sys/kernel/debug/omap_mux/sad2d_swrite
/sys/kernel/debug/omap_mux/sdmmc1_clk
/sys/kernel/debug/omap_mux/sdmmc1_cmd
/sys/kernel/debug/omap_mux/sdmmc1_dat0
/sys/kernel/debug/omap_mux/sdmmc1_dat1
/sys/kernel/debug/omap_mux/sdmmc1_dat2
/sys/kernel/debug/omap_mux/sdmmc1_dat3
/sys/kernel/debug/omap_mux/sdmmc2_clk
/sys/kernel/debug/omap_mux/sdmmc2_cmd
/sys/kernel/debug/omap_mux/sdmmc2_dat0
/sys/kernel/debug/omap_mux/sdmmc2_dat1
/sys/kernel/debug/omap_mux/sdmmc2_dat2
/sys/kernel/debug/omap_mux/sdmmc2_dat3
/sys/kernel/debug/omap_mux/sdmmc2_dat4
/sys/kernel/debug/omap_mux/sdmmc2_dat5
/sys/kernel/debug/omap_mux/sdmmc2_dat6
/sys/kernel/debug/omap_mux/sdmmc2_dat7
/sys/kernel/debug/omap_mux/sdrc_cke0
/sys/kernel/debug/omap_mux/sdrc_cke1
/sys/kernel/debug/omap_mux/sim_clk
/sys/kernel/debug/omap_mux/sim_io
/sys/kernel/debug/omap_mux/sim_pwrctrl
/sys/kernel/debug/omap_mux/sim_rst
/sys/kernel/debug/omap_mux/sys_boot0
/sys/kernel/debug/omap_mux/sys_boot1
/sys/kernel/debug/omap_mux/sys_boot2
/sys/kernel/debug/omap_mux/sys_boot3
/sys/kernel/debug/omap_mux/sys_boot4
/sys/kernel/debug/omap_mux/sys_boot5
/sys/kernel/debug/omap_mux/sys_boot6
/sys/kernel/debug/omap_mux/sys_clkout1
/sys/kernel/debug/omap_mux/sys_clkout2
/sys/kernel/debug/omap_mux/sys_clkreq
/sys/kernel/debug/omap_mux/sys_nirq
/sys/kernel/debug/omap_mux/sys_nreswarm
/sys/kernel/debug/omap_mux/sys_off_mode
/sys/kernel/debug/omap_mux/uart1_cts
/sys/kernel/debug/omap_mux/uart1_rts
/sys/kernel/debug/omap_mux/uart1_rx
/sys/kernel/debug/omap_mux/uart1_tx
/sys/kernel/debug/omap_mux/uart2_cts
/sys/kernel/debug/omap_mux/uart2_rts
/sys/kernel/debug/omap_mux/uart2_rx
/sys/kernel/debug/omap_mux/uart2_tx
/sys/kernel/debug/omap_mux/uart3_cts_rctx
/sys/kernel/debug/omap_mux/uart3_rts_sd
/sys/kernel/debug/omap_mux/uart3_rx_irrx
/sys/kernel/debug/omap_mux/uart3_tx_irtx
*/