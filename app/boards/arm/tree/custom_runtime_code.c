/*
 * Copyright (c) 2022 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/settings/settings.h>
#include <zephyr/sys/sys_io.h>
#include <zephyr/devicetree.h>

static const struct gpio_dt_spec led_r = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios,
						     {0});
static const struct gpio_dt_spec led_g = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led1), gpios,
						     {0});
static const struct gpio_dt_spec led_b = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led2), gpios,
						     {0});
static const struct gpio_dt_spec led_conn = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led3), gpios,
						     {0});
static const struct gpio_dt_spec reset_out = GPIO_DT_SPEC_GET_OR(DT_ALIAS(resetout), gpios,
						     {0});
static const struct gpio_dt_spec reset_in = GPIO_DT_SPEC_GET_OR(DT_ALIAS(resetin), gpios,
						     {0});



static void setLedsDisconnected() {
	int err = gpio_pin_set_dt(&led_conn, 1);
//	err = gpio_pin_set_dt(&led_r, 0);
//	err = gpio_pin_set_dt(&led_g, 0);
//	err = gpio_pin_set_dt(&led_b, 0);

	if (err) {
		printk("LED Set failed (err 0x%02x)\n", err);
	}
}

static void setLedsConnected() {
	int err = gpio_pin_set_dt(&led_conn, 0);
	err = gpio_pin_set_dt(&led_r, 0);
	err = gpio_pin_set_dt(&led_g, 0);
	err = gpio_pin_set_dt(&led_b, 0);
	
	if (err) {
		printk("LED Set failed (err 0x%02x)\n", err);
	}
}

/**
 * When the BT is connected, turn off an LED
 */
static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		printk("Connection failed (err 0x%02x)\n", err);
		setLedsDisconnected();
	} else {
		setLedsConnected();
		printk("Connected\n");
	}
}

/**
 * When the BT is disconnected, turn on an LED
 */
static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	setLedsDisconnected();
	printk("Disconnected (reason 0x%02x)\n", reason);
}

// Configure the connection callbacks
BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

#if IS_ENABLED(CONFIG_SETTINGS)

static int peripheral_ble_handle_set(const char *name, size_t len, settings_read_cb read_cb,
                                     void *cb_arg) {
    return 0;
}

static int zmk_peripheral_ble_complete_startup(void) {
	gpio_pin_set_dt(&reset_out, 1);
	int ret = gpio_pin_configure_dt(&led_b, gpio_pin_get_dt(&reset_in) == 1 ? GPIO_OUTPUT_ACTIVE : GPIO_OUTPUT_INACTIVE);
	ret = gpio_pin_configure_dt(&led_r, GPIO_OUTPUT_INACTIVE);
	ret = gpio_pin_configure_dt(&led_g, GPIO_OUTPUT_INACTIVE);
 
	if (gpio_pin_get_dt(&reset_in) == 1) {
     	bt_unpair(BT_ID_DEFAULT, NULL);
	}
    return 0;
}


static struct settings_handler ble_peripheral_settings_handler = {
    .name = "ble_customl",
    .h_set = peripheral_ble_handle_set,
    .h_commit = zmk_peripheral_ble_complete_startup};

#endif // IS_ENABLED(CONFIG_SETTINGS)

static int init(const struct device *port) {
	printk("HEllooo tree start\n");
	int ret = gpio_pin_configure_dt(&led_conn, GPIO_OUTPUT_ACTIVE);
	
    int err = bt_enable(NULL);

#if IS_ENABLED(CONFIG_SETTINGS)
    settings_register(&ble_peripheral_settings_handler);
#else
    zmk_peripheral_ble_complete_startup();
#endif
	gpio_pin_set_dt(&reset_out, 0);
	return 0;	
}

SYS_INIT(init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
