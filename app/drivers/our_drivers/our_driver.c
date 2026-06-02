


#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>

#define DT_DRV_COMPAT our_driver

#define LED_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF );


static int our_channel_get(const struct device *dev,
				            enum sensor_channel chan,
				            struct sensor_value *val) {


    LOG_INF("Hello from Channel Get, channel %d", chan);
    LOG_INF("LED off.");

    gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    
    return 0;
}


static int our_sample_fetch(const struct device *dev,
                             enum sensor_channel chan){

    LOG_INF("Hello from our driver fetch, channel %d", chan);
    LOG_INF("LED on.");

     gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    
    return 0;
}

static DEVICE_API(sensor, api_iomico_lecture) = {

    .channel_get = our_channel_get,
    .sample_fetch = our_sample_fetch,
};



static int init(const struct device *dev) {


    if (!gpio_is_ready_dt(&led)) {
        LOG_ERR("LED GPIO not ready");
        return -ENODEV;
    }

    int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    if (ret != 0){
        LOG_ERR("Failed to configure LED GPIO, error %d", ret);
        return ret;
    }

    LOG_INF("Device driver initialized.");
    return 0;
}





DEVICE_DT_INST_DEFINE(0, init, NULL, NULL, NULL, POST_KERNEL, 80, 
                       &api_iomico_lecture);