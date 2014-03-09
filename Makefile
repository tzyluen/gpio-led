CC=gcc
CFLAGS=-std=c99 -O0 -g -Wall

default: gpio_led gpio_sysfs

gpio_sysfs:
	$(CC) $(CFLAGS) gpio_sysfs.c -o gpio_sysfs

gpio_led:
	$(CC) $(CFLAGS) gpio_led.c -o gpio_led

clean:
	rm -f *.o a.out gpio_led gpio_sysfs
