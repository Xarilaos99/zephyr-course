#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>


#include "../drivers/our_drivers/our_driver.h"
// #define SLEEP_TIME_MS 1000


/* The devicetree node identifier for the "app_led(app-led)" alias. */
#define LED_NODE0 DT_ALIAS(app_led)
#define LED_NODE DT_ALIAS(led0)



static const struct device * driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED_NODE0, gpios);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);



int main(void)
{
    bool led_state = true;
    uint8_t state = 0;

    if (!gpio_is_ready_dt(&led0)) return 0;
    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE) < 0) return 0;
    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    LOG_INF("Hello World !!!");
    
    while (1) {
        // LOG_INF("Hello World !!!");

        if (gpio_pin_toggle_dt(&led) < 0) return 0;


        if(led_state) sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, NULL);
        else sensor_sample_fetch(driver);

        led_state = !led_state;

        state++;


        if(state == 5)
        {
            our_driver_set_param(driver, 2);
        }
        if(state >= 10)
        {
            our_driver_set_param(driver, 0);
            state = 0;
        }


        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        // k_msleep(SLEEP_TIME_MS);
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);

    }
    return 0;
}
