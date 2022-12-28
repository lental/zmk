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

/**
 * When the BT is connected, turn on an LED
 */
static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		printk("Connection failed (err 0x%02x)\n", err);
		int err = gpio_pin_set_dt(&led_two, 0);
		if (err) {
			printk("LED Set failed (err 0x%02x)\n", err);
		}
	} else {
		int err = gpio_pin_set_dt(&led_two, 1);
		if (err) {
			printk("LED Set failed (err 0x%02x)\n", err);
		}
		printk("Connected\n");
	}
}

/**
 * When the BT is disconnected, turn o ff an LED
 */
static void disconnected(struct bt_conn *conn, uint8_t reason)
{
		int err = gpio_pin_set_dt(&led_two, 0);
		if (err) {
			printk("LED Set failed (err 0x%02x)\n", err);
		}
	printk("Disconnected (reason 0x%02x)\n", reason);
}

// Configure the connection callbacks
BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};


static int init(const struct device *port) {
		printk("HEllooo tree startho\n");
	int ret = gpio_pin_configure_dt(&led_one, GPIO_OUTPUT_ACTIVE);
	ret = gpio_pin_configure_dt(&led_two, GPIO_OUTPUT_INACTIVE);
	ret = gpio_pin_configure_dt(&led_three, GPIO_OUTPUT_INACTIVE);

	return 0;	
}

SYS_INIT(init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
