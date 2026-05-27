/*
 * Copyright (c) 2026 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/settings/settings.h>
#include <zephyr/sys/printk.h>
#include <stdio.h>

#if IS_ENABLED(CONFIG_SETTINGS)

static int central_custom_ble_handle_set(const char *name, size_t len, settings_read_cb read_cb,
                                         void *cb_arg) {
    return 0;
}

static int zmk_central_ble_clear_bonds_on_start(void) {
    printk("Nice Nano Custom Runtime: Unconditionally clearing all BLE bonds on boot...\n");

    // Unpair all devices
    int unpair_err = bt_unpair(BT_ID_DEFAULT, NULL);
    if (unpair_err) {
        printk("Failed to unpair devices (err %d)\n", unpair_err);
    }

    // Delete saved host profiles settings
    for (int i = 0; i < 8; i++) {
        char setting_name[16];
        snprintf(setting_name, sizeof(setting_name), "ble/profiles/%d", i);
        int err = settings_delete(setting_name);
        if (err && err != -ENOENT) {
            printk("Failed to delete settings profile %s (err %d)\n", setting_name, err);
        }
    }

    // Delete saved split peripheral addresses settings
    for (int i = 0; i < 8; i++) {
        char setting_name[32];
        snprintf(setting_name, sizeof(setting_name), "ble/peripheral_addresses/%d", i);
        int err = settings_delete(setting_name);
        if (err && err != -ENOENT) {
            printk("Failed to delete settings peripheral address %s (err %d)\n", setting_name, err);
        }
    }

    printk("Nice Nano Custom Runtime: BLE bonds cleared successfully\n");
    return 0;
}

static struct settings_handler ble_central_custom_settings_handler = {
    .name = "ble_customc",
    .h_set = central_custom_ble_handle_set,
    .h_commit = zmk_central_ble_clear_bonds_on_start
};

#endif // IS_ENABLED(CONFIG_SETTINGS)

static int init(void) {
    printk("Nice Nano Custom Runtime: initializing custom boot logic\n");
#if IS_ENABLED(CONFIG_SETTINGS)
    int err = settings_register(&ble_central_custom_settings_handler);
    if (err) {
        printk("Failed to register settings handler for custom runtime (err %d)\n", err);
    }
#else
    zmk_central_ble_clear_bonds_on_start();
#endif
    return 0;
}

SYS_INIT(init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
