/* net.c - KNoT Application Client */

/*
 * Copyright (c) 2018, CESAR. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * The knot client application is acting as a client that is run in Zephyr OS,
 * The client sends sensor data encapsulated using KNoT netcol.
 */

//New modifications by *Lucas Asafe*
	/*
	-all 3 buttons and leds turned on
	-setting time for the leds to turn off (e.g 3000ms and the the led simply turns off)
	-etc
	*/

#include <zephyr.h>
#include <net/net_core.h>
#include <logging/log.h>

#include "knot.h"
#include "knot_types.h"
#include "knot_protocol.h"

LOG_MODULE_DECLARE(knot, LOG_LEVEL_DBG);

/* Tracked values */

static bool led = true;
static bool led2 = true;
static bool led3 = true;
/*
 * Use GPIO only for real boards.
 * Use timer to mock values change if using qemu.
 */
#if CONFIG_BOARD_NRF52840_PCA10056
#include <device.h>
#include <gpio.h>
#define GPIO_PORT		SW0_GPIO_CONTROLLER /* General GPIO Controller */
#define GPIO_PORT1		SW1_GPIO_CONTROLLER /* General GPIO Controller */
#define GPIO_PORT2		SW2_GPIO_CONTROLLER /* General GPIO Controller */
#define BUTTON_PIN		DT_GPIO_KEYS_SW0_GPIO_PIN /* botao 1 */
#define BUTTON_PIN2		DT_GPIO_KEYS_SW1_GPIO_PIN /* botao 2 */
#define LED_PIN			13 /* led 1 */
#define LED_PIN2		14 /* led 2 */
#define LED_PIN3		15 /* led 3 */

static struct device *gpiob;		/* GPIO device */
static struct device *gpiob2;		/* GPIO device */
static struct device *gpiob3;
static struct gpio_callback button_cb; /* Button pressed callback */
static struct gpio_callback button_cb2; /* Button pressed callback */
static struct gpio_callback button_cb3; /* Button pressed callback */

static void btn_press(struct device *gpiob,
		       struct gpio_callback *cb, u32_t pins)
{
	led = !led;
	gpio_pin_write(gpiob, LED_PIN, !led); /* Update GPIO */

}
static void btn_press2(struct device *gpiob,
		       struct gpio_callback *cb, u32_t pins)
{
	led2 = !led2;
	gpio_pin_write(gpiob, LED_PIN2, !led2); /* Update GPIO */

}
static void btn_press3(struct device *gpiob,
		       struct gpio_callback *cb, u32_t pins)
{
	led3 = !led3;
	gpio_pin_write(gpiob, LED_PIN3, !led3); /* Update GPIO */

}
#elif CONFIG_BOARD_QEMU_X86




#define UPDATE_PERIOD K_SECONDS(3) /* Update values each 3 seconds */
static void val_update(struct k_timer *timer_id)
{
	led = !led;
	k_timer_start(timer_id, UPDATE_PERIOD, UPDATE_PERIOD);
}
K_TIMER_DEFINE(val_update_timer, val_update, NULL);
static void val_update2(struct k_timer *timer_id)
{
	led2 = !led2;
	k_timer_start(timer_id, UPDATE_PERIOD, UPDATE_PERIOD);
}
K_TIMER_DEFINE(val_update_timer, val_update, NULL);
#endif


static void changed_led(struct knot_proxy *proxy)
{
	knot_proxy_value_get_basic(proxy, &led);
	LOG_INF("Value for led changed to %d", led);

#if CONFIG_BOARD_NRF52840_PCA10056
	gpio_pin_write(gpiob, LED_PIN, !led); /* Led is On at LOW */
#endif
}
static void changed_led2(struct knot_proxy *proxy)
{
	knot_proxy_value_get_basic(proxy, &led2);
	LOG_INF("Value for led changed to %d", led2);

#if CONFIG_BOARD_NRF52840_PCA10056
	gpio_pin_write(gpiob2, LED_PIN2, !led2); /* Led is On at LOW */
#endif
}
static void changed_led3(struct knot_proxy *proxy)
{
	knot_proxy_value_get_basic(proxy, &led3);
	LOG_INF("Value for led changed to %d", led3);

#if CONFIG_BOARD_NRF52840_PCA10056
	gpio_pin_write(gpiob3, LED_PIN3, !led3); /* Led is On at LOW */
#endif
}
static void poll_led(struct knot_proxy *proxy)
{
	/* Pushing status to remote */
	bool res;
	res = knot_proxy_value_set_basic(proxy, &led);

	/* Notify if sent */
	if (res) {
		if (led)
			LOG_INF("Sending value true for led");
		else
			LOG_INF("Sending value false for led");
	}
}
static void poll_led2(struct knot_proxy *proxy)
{
	/* Pushing status to remote */
	bool res;
	res = knot_proxy_value_set_basic(proxy, &led2);

	/* Notify if sent */
	if (res) {
		if (led2)
			LOG_INF("Sending value true for led2");
		else
			LOG_INF("Sending value false for led2");
	}
}
static void poll_led3(struct knot_proxy *proxy)
{
	/* Pushing status to remote */
	bool res;
	res = knot_proxy_value_set_basic(proxy, &led3);

	/* Notify if sent */
	if (res) {
		if (led3)
			LOG_INF("Sending value true for led3");
		else
			LOG_INF("Sending value false for led3");
	}
}

void setup(void)
{
	bool success;

	/* BUTTON - Sent after change */
	if (knot_proxy_register(1, "LED", KNOT_TYPE_ID_SWITCH,
		      KNOT_VALUE_TYPE_BOOL, KNOT_UNIT_NOT_APPLICABLE,
		      changed_led, poll_led) == NULL) {
		LOG_ERR("LED failed to register");
	}
	success = knot_proxy_set_config(1, KNOT_EVT_FLAG_CHANGE, NULL);
	if (!success)
		LOG_ERR("LED failed to configure");

		/* BUTTON - Sent after change */
	if (knot_proxy_register(2, "LED2", KNOT_TYPE_ID_SWITCH,
		      KNOT_VALUE_TYPE_BOOL, KNOT_UNIT_NOT_APPLICABLE,
		      changed_led2, poll_led2) == NULL) {
		LOG_ERR("LED failed to register");
	}
	success = knot_proxy_set_config(2, KNOT_EVT_FLAG_CHANGE, NULL);
	if (!success)
		LOG_ERR("LED2 failed to configure");

	/* BUTTON - Sent after change */
	if (knot_proxy_register(3, "LED3", KNOT_TYPE_ID_SWITCH,
		      KNOT_VALUE_TYPE_BOOL, KNOT_UNIT_NOT_APPLICABLE,
		      changed_led3, poll_led3) == NULL) {
		LOG_ERR("LED failed to register");
	}
	success = knot_proxy_set_config(3, KNOT_EVT_FLAG_CHANGE, NULL);
	if (!success)
		LOG_ERR("LED3 failed to configure");

	/* Peripherals control */
#if CONFIG_BOARD_NRF52840_PCA10056
	/* Read button */
	gpiob = device_get_binding(GPIO_PORT);
	/* Button Pin has pull up, interruption on low edge and debounce */
	gpio_pin_configure(gpiob, BUTTON_PIN,
			   GPIO_DIR_IN | GPIO_PUD_PULL_UP | GPIO_INT_DEBOUNCE |
			   GPIO_INT | GPIO_INT_EDGE | GPIO_INT_ACTIVE_HIGH);
	gpio_init_callback(&button_cb, btn_press, BIT(BUTTON_PIN));
	gpio_add_callback(gpiob, &button_cb);
	gpio_pin_enable_callback(gpiob, BUTTON_PIN);

	gpiob2 = device_get_binding(GPIO_PORT1);
	/* Button Pin has pull up, interruption on low edge and debounce */
	gpio_pin_configure(gpiob2, BUTTON_PIN2,
			   GPIO_DIR_IN | GPIO_PUD_PULL_UP | GPIO_INT_DEBOUNCE |
			   GPIO_INT | GPIO_INT_EDGE | GPIO_INT_ACTIVE_LOW);
	gpio_init_callback(&button_cb2, btn_press2, BIT(BUTTON_PIN2));
	gpio_add_callback(gpiob2, &button_cb2);
	gpio_pin_enable_callback(gpiob2, BUTTON_PIN2);

	gpiob2 = device_get_binding(GPIO_PORT1);
	/* Button Pin has pull up, interruption on low edge and debounce */
	gpio_pin_configure(gpiob2, BUTTON_PIN2,
			   GPIO_DIR_IN | GPIO_PUD_PULL_UP | GPIO_INT_DEBOUNCE |
			   GPIO_INT | GPIO_INT_EDGE | GPIO_INT_ACTIVE_LOW);
	gpio_init_callback(&button_cb2, btn_press2, BIT(BUTTON_PIN2));
	gpio_add_callback(gpiob2, &button_cb2);
	gpio_pin_enable_callback(gpiob2, BUTTON_PIN2);

	/* Led pin */
	gpio_pin_configure(gpiob, LED_PIN, GPIO_DIR_OUT);
	gpio_pin_configure(gpiob2, LED_PIN2, GPIO_DIR_OUT);

#elif CONFIG_BOARD_QEMU_X86
	k_timer_start(&val_update_timer, UPDATE_PERIOD, UPDATE_PERIOD);
#endif

}

void loop(void)
{
}
