#include <linux/module.h>
#include <linux/timer.h>
#include <linux/device.h>
#include <linux/err.h>
#include <asm/io.h>
#include <mach/platform.h>
#include "kgpio_led.h"

MODULE_AUTHOR("TzyLuen Ng <tzyluen.ng@gmail.com>");
MODULE_DESCRIPTION("GPIO LKM LED");
MODULE_LICENSE("GPL");


static DEVICE_ATTR(period, S_IWUSR | S_IWGRP | S_IWOTH, NULL, set_period_callback);
static int __init kgpio_led_init(void)
{
    int result;
    /**
     * set `sp_gpio_register' point to the address of the GPIO registers in memory.
     * `GPIO_BASE' is the base address of the GPIO registers defined in `platform.h'.
     */
    sp_gpio_registers = (t_gpio_registers *) __io_address(GPIO_BASE);
    set_gpio_function(led_gpio_pin, 0b001);     /* output */

    setup_timer(&s_blink_timer, blink_timer_handler, 0);
    /* jiffies: a global variable holds the number of ticks since system booted. */
    result = mod_timer(&s_blink_timer, jiffies + msecs_to_jiffies(s_blink_period));

    BUG_ON(result < 0);

    sp_device_class = class_create(THIS_MODULE, "kgpio_led");
    BUG_ON(IS_ERR(sp_device_class));

    sp_device_object = device_create(sp_device_class, NULL, 0, NULL, "kgpio_led");
    BUG_ON(IS_ERR(sp_device_object));

    result = device_create_file(sp_device_object, &dev_attr_period);
    BUG_ON(result < 0);

    return 0;
}


static void __exit kgpio_led_exit(void)
{
    device_remove_file(sp_device_object, &dev_attr_period);
    device_destroy(sp_device_class, 0);
    class_destroy(sp_device_class);

    set_gpio_function(led_gpio_pin, 0); /* configure `led_gpio_pin` as input */
    del_timer(&s_blink_timer);  /* deactive the timer */
}


static void set_gpio_function(int gpio, int function_code)
{
    int register_index = gpio / 10;
    int bit = (gpio % 10) * 3;

    unsigned old_value = sp_gpio_registers->GPFSEL[register_index];
    unsigned mask = 0b111 << bit;
    printk("Changing function of GPIO%d from %x to %x\n",
            gpio, (old_value >> bit) & 0b111, function_code);

    sp_gpio_registers->GPFSEL[register_index]
            = (old_value & ~mask) | ((function_code << bit) & mask);
}


static void set_gpio_output_value(int gpio, bool output_value)
{
    if (output_value)
        sp_gpio_registers->GPSET[gpio / 32] = (1 << (gpio % 32));
    else
        sp_gpio_registers->GPCLR[gpio / 32] = (1 << (gpio % 32));
}


static void blink_timer_handler(unsigned long unused)
{
    static bool on = false;
    on = !on;
    set_gpio_output_value(led_gpio_pin, on);
    mod_timer(&s_blink_timer, jiffies + msecs_to_jiffies(s_blink_period));
}


static ssize_t set_period_callback(struct device *dev, struct device_attribute *attr,
        const char *buf, size_t count)
{
    long period_value = 0;
    if (kstrtol(buf, 10, &period_value) < 0)
        return -EINVAL;
    if (period_value < 10) /* safety check */
        return -EINVAL;

    s_blink_period = period_value;
    return count;
}


module_init(kgpio_led_init);
module_exit(kgpio_led_exit);
