#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/leds.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>

#define DRIVER_NAME "drvled"

#define LED_OFF 0
#define LED_ON 1

#define GPIO_NUM 9

struct drvled_data_st
{
    struct gpio_desc *desc;
    struct led_classdev led_cdev;
};

static struct drvled_data_st *drvled_data;

static void drvled_setled(unsigned int status)
{
    if (status == LED_ON)
    {
        gpiod_set_value(drvled_data->desc, 1);
    }
    else
    {
        gpiod_set_value(drvled_data->desc, 0);
    }
}

static void drvled_change_state(struct led_classdev *led_cdev, enum led_brightness brightness)
{
    if (brightness)
    {
        drvled_setled(LED_ON);
    }
    else
    {
        drvled_setled(LED_OFF);
    }
}

static int __init drvled_init(void)
{
    int result = 0;

    drvled_data = kzalloc(sizeof(*drvled_data), GFP_KERNEL);
    if (!drvled_data)
    {
        result = -ENOMEM;
        goto ret_err_kzalloc;
    }

    result = gpio_request(GPIO_NUM, DRIVER_NAME);
    if (result)
    {
        pr_err("%s: Error requesting GPIO\n", DRIVER_NAME);
        goto ret_err_gpio_request;
    }

    drvled_data->desc = gpio_to_desc(GPIO_NUM);

    drvled_data->led_cdev.name = "ipe:red:user";
    drvled_data->led_cdev.brightness_set = drvled_change_state;

    result = led_classdev_register(NULL, &drvled_data->led_cdev);
    if (result)
    {
        pr_err("%s: Error registering led\n", DRIVER_NAME);
        goto ret_err_led_classdev_register;
    }

    gpiod_direction_output(drvled_data->desc, 0);

    pr_info("%s: initialized.\n", DRIVER_NAME);
    goto ret_ok;

ret_err_led_classdev_register:
    gpio_free(GPIO_NUM);
ret_err_gpio_request:
    kfree(drvled_data);
ret_err_kzalloc:
ret_ok:
    return result;
}

static void __exit drvled_exit(void)
{
    led_classdev_unregister(&drvled_data->led_cdev);
    gpio_free(GPIO_NUM);
    kfree(drvled_data);
    pr_info("%s: exiting.\n", DRIVER_NAME);
}

module_init(drvled_init);
module_exit(drvled_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Max Tong <maxtong198776@gmail.com>");
MODULE_VERSION("1.0");
