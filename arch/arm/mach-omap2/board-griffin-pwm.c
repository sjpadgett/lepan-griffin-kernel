
static void griffin_pwm_config(u8 brightness) {
    u8 pwm_off = 0;
    
    pwm_off = (MIN_CYCLES * (LED_FULL - brightness) +
    MAX_CYCLES * (brightness - LED_OFF)) /
    (LED_FULL - LED_OFF);
    
    pwm_off = clamp(pwm_off, (u8)MIN_CYCLES, (u8)MAX_CYCLES);
    
    /* start at 0 */
    twl_i2c_write_u8(TWL4030_MODULE_PWM0, 0, 0);
    twl_i2c_write_u8(TWL4030_MODULE_PWM0, pwm_off, 1);
}

static void griffin_pwm_enable(int enable) {
    u8 gpbr1;
    
    twl_i2c_read_u8(TWL4030_MODULE_INTBR, &gpbr1, REG_INTBR_GPBR1);
    gpbr1 &= ~REG_INTBR_GPBR1_PWM0_OUT_EN_MASK;
    gpbr1 |= (enable ? REG_INTBR_GPBR1_PWM0_OUT_EN : 0);
    twl_i2c_write_u8(TWL4030_MODULE_INTBR, gpbr1, REG_INTBR_GPBR1);
    
    twl_i2c_read_u8(TWL4030_MODULE_INTBR, &gpbr1, REG_INTBR_GPBR1);
    gpbr1 &= ~REG_INTBR_GPBR1_PWM0_CLK_EN_MASK;
    gpbr1 |= (enable ? REG_INTBR_GPBR1_PWM0_CLK_EN : 0);
    twl_i2c_write_u8(TWL4030_MODULE_INTBR, gpbr1, REG_INTBR_GPBR1);
}

static void griffin_set_primary_brightness(u8 brightness) {
    u8 pmbr1;
    static int griffin_pwm0_config;
    static int griffin_pwm0_output_enabled;
    
    if (griffin_pwm0_config == 0) {
	twl_i2c_read_u8(TWL4030_MODULE_INTBR, &pmbr1, REG_INTBR_PMBR1);
	
	pmbr1 &= ~REG_INTBR_PMBR1_PWM0_PIN_MASK;
	pmbr1 |=  REG_INTBR_PMBR1_PWM0_PIN_EN;
	twl_i2c_write_u8(TWL4030_MODULE_INTBR, pmbr1, REG_INTBR_PMBR1);
	
	griffin_pwm0_config = 1;
    }
    
    if (!brightness) {
	griffin_pwm_enable(0);
	griffin_pwm0_output_enabled = 0;
	return;
    }
    
    griffin_pwm_config(brightness);
    if (griffin_pwm0_output_enabled == 0) {
	griffin_pwm_enable(1);
	griffin_pwm0_output_enabled = 1;
    }
}

static struct omap4430_sdp_disp_led_platform_data griffin_disp_led_data = {
    .flags = LEDS_CTRL_AS_ONE_DISPLAY,
    .primary_display_set = griffin_set_primary_brightness,
    .secondary_display_set = NULL,
};

static struct platform_device griffin_disp_led = {
    .name   =       "display_led",
    .id     =       -1,
    .dev    = {
	.platform_data = &griffin_disp_led_data,
    },
};
