/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "blink.h"

extern struct k_fifo printk_fifo;

void blink(const struct led *led, uint32_t sleep_ms, uint32_t id)
{
    const struct device *gpio_dev;
    int cnt = 0;
    int ret;

    gpio_dev = device_get_binding(led->gpio_dev_name);
    if (gpio_dev == NULL) {
        printk("Error: didn't find %s device\n", led->gpio_dev_name);
        return;
    }

    ret = gpio_pin_configure(gpio_dev, led->gpio_pin, led->gpio_flags);
    if (ret != 0) {
        printk("Error %d: failed to configure pin %d '%s'\n", ret, led->gpio_pin,
               led->gpio_pin_name);
        return;
    }

    while (1) {
        gpio_pin_set(gpio_dev, led->gpio_pin, cnt % 2);

        struct printk_data_t tx_data = {.led = id, .cnt = cnt};

        size_t size   = sizeof(struct printk_data_t);
        char *mem_ptr = k_malloc(size);
        __ASSERT_NO_MSG(mem_ptr != 0);

        memcpy(mem_ptr, &tx_data, size);

        k_fifo_put(&printk_fifo, mem_ptr);

        k_msleep(sleep_ms);
        cnt++;
    }
}

void blink0(void)
{
    const struct led led0 = {
#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
        .gpio_dev_name = DT_GPIO_LABEL(LED0_NODE, gpios),
        .gpio_pin_name = DT_LABEL(LED0_NODE),
        .gpio_pin      = DT_GPIO_PIN(LED0_NODE, gpios),
        .gpio_flags    = GPIO_OUTPUT | DT_GPIO_FLAGS(LED0_NODE, gpios),
#else
#error "Unsupported board: led0 devicetree alias is not defined"
#endif
    };

    blink(&led0, 100, 0);
}

void blink1(void)
{
    const struct led led1 = {
#if DT_NODE_HAS_STATUS(LED1_NODE, okay)
        .gpio_dev_name = DT_GPIO_LABEL(LED1_NODE, gpios),
        .gpio_pin_name = DT_LABEL(LED1_NODE),
        .gpio_pin      = DT_GPIO_PIN(LED1_NODE, gpios),
        .gpio_flags    = GPIO_OUTPUT | DT_GPIO_FLAGS(LED1_NODE, gpios),
#else
#error "Unsupported board: led1 devicetree alias is not defined"
#endif
    };

    blink(&led1, 1000, 1);
}
