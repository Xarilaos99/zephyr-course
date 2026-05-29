#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// #define SLEEP_TIME_MS 1000


#ifdef CONFIG_LED_SUBSYSTEM
/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

#endif

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);


int main(void)
{
    bool led_state = true;

    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    while (1) {



#ifdef CONFIG_LED_SUBSYSTEM
        if (gpio_pin_toggle_dt(&led) < 0) return 0;


        led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        // k_msleep(SLEEP_TIME_MS);
        k_msleep(CONFIG_BLINK_SLEEP_TIME_MS);


#ifdef CONFIG_LED_ADVANCED

LOG_INF("LED advanced information: Led Brightness %d,Led Fade Duration %d", CONFIG_LED_BRIGHTNESS,CONFIG_LED_FADE_DURATION);


if(IS_ENABLED(CONFIG_ENABLE_LED_DEBUGGING))
{
LOG_INF("LED debugging is enabled");

}

if(IS_ENABLED(CONFIG_CUSTOM_BLINK_PATTERN))
{
LOG_INF("Custom blink pattern is enabled");
}
#endif

#else
    LOG_INF("No LED Subsystem Enabled!");
#endif
    }
    return 0;
}
