


#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>

#define DT_DRV_COMPAT our_driver

#define LED_NODE0 DT_ALIAS(app_led)
#define LED_NODE DT_ALIAS(led0)

typedef struct our_driver_data {
    uint32_t param0;
} our_driver_data_t;

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE0, gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF );


static int our_channel_get(const struct device *dev,
				            enum sensor_channel chan,
				            struct sensor_value *val) {


    our_driver_data_t *drv_data = (our_driver_data_t *)dev->data;
    
    LOG_INF("Hello from Channel Get, channel %d", chan);
    if(drv_data->param0 == 0) gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    else gpio_pin_configure_dt(&led2, GPIO_OUTPUT_INACTIVE);
    LOG_INF("LED off.");

    
    return 0;
}


static int our_sample_fetch(const struct device *dev,
                             enum sensor_channel chan){

    our_driver_data_t *drv_data = (our_driver_data_t *)dev->data;

    LOG_INF("Hello from our driver fetch, channel %d", chan);
    if(drv_data->param0 == 0) gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    else gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
    LOG_INF("LED on.");

    
    return 0;
}

static DEVICE_API(sensor, api_iomico_lecture) = {

    .channel_get = our_channel_get,
    .sample_fetch = our_sample_fetch,
};



static int init(const struct device *dev) {

    our_driver_data_t *drv_data = (our_driver_data_t *)dev->data;

    if (!gpio_is_ready_dt(&led)) {
        LOG_ERR("LED GPIO not ready");
        return -ENODEV;
    }

    int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    if (ret != 0){
        LOG_ERR("Failed to configure LED GPIO, error %d", ret);
        return ret;
    }

    if (NULL != drv_data)
    {
        drv_data->param0 = 0;
        LOG_INF("param1 initialized to 0");
    }
    else
    {
        LOG_ERR("Device data is NULL");
    }

    LOG_INF("Device driver initialized.");
    return 0;
}



void our_driver_set_param(const struct device *dev, uint32_t param)
{
    our_driver_data_t *drv_data = (our_driver_data_t *)dev->data;

    if (NULL != drv_data)
    {
        drv_data->param0 = param;
        LOG_INF("Change param0 to %u", param);
    }
    else
    {
        LOG_ERR("Device data is NULL");
    }
}


// Macro to enable multiple instances of the driver based on the device tree.
#define DEV_INST(inst)                                        \
   static our_driver_data_t our_driver_data_##inst;     \                                                   
    DEVICE_DT_INST_DEFINE(inst, init, NULL,    \
                          &our_driver_data_##inst,NULL, POST_KERNEL, 80, &api_iomico_lecture);

DT_INST_FOREACH_STATUS_OKAY(DEV_INST)

