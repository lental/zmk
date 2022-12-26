/*
 * Copyright (c) 2022 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <bluetooth/conn.h>
#include <kernel.h>
#include <device.h>
#include <init.h>
#include <drivers/gpio.h>
#include <sys/sys_io.h>
#include <devicetree.h>

static const struct gpio_dt_spec led_one = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios,
						     {0});
static const struct gpio_dt_spec led_two = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led1), gpios,
						     {0});
static const struct gpio_dt_spec led_three = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led2), gpios,
						     {0});

static int init(const struct device *port) {
		printk("HEllooo tree startho\n");
	int ret = gpio_pin_configure_dt(&led_one, GPIO_OUTPUT_ACTIVE);
	ret = gpio_pin_configure_dt(&led_two, GPIO_OUTPUT_INACTIVE);
	ret = gpio_pin_configure_dt(&led_three, GPIO_OUTPUT_ACTIVE);
	return 0;	
}

SYS_INIT(init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
