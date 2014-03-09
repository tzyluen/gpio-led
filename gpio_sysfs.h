#ifndef GPIO_SYSFS_H_
#define GPIO_SYSFS_H_

#define IN               0
#define OUT              1
#define LOW              0
#define HIGH             1
#define PIN             24     /* P1-18 */
#define POUT             4     /* P1-07 */

#define BUFFER_MAX       3
#define DIRECTION_MAX   35
#define VALUE_MAX       30

static int gpio_export(int pin);
static int gpio_unexport(int pin);
static int gpio_direction(int pin, int dir);
static int gpio_read(int pin);
static int gpio_write(int pin, int value);

#endif
