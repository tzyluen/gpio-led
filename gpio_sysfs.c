#define _BSD_SOURCE     /* suppress 'implicit declaration of function `usleep`' warning */

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gpio_sysfs.h"


/**
 * blinks GPIO 4 (P1-07) while reading GPIO 24 (P1-18)
 */
int main(int argc, char **argv)
{
    int repeat = 10;

    /* enable GPIO pins */
    if (-1 == gpio_export(POUT) || -1 == gpio_export(PIN))
        return EXIT_FAILURE-1;

    /* set GPIO directions */
    if (-1 == gpio_direction(POUT, OUT) || -1 == gpio_direction(PIN, IN))
        return EXIT_FAILURE-2;

    do {
        /* write GPIO value */
        if (-1 == gpio_write(POUT, repeat % 2))
            return EXIT_FAILURE-3;

        /* read GPIO value */
        printf("reading %d in GPIO %d\n", gpio_read(PIN), PIN);
        usleep(500 * 1000);

    } while (repeat--);

    /* disable GPIO pins */
    if (-1 == gpio_unexport(POUT) || -1 == gpio_unexport(PIN))
        return EXIT_FAILURE-4;
    
    return EXIT_SUCCESS;
}


static int gpio_export(int pin)
{
    char buffer[BUFFER_MAX];
    ssize_t bytes_written;
    int fd;

    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open export for writing.\n");
        return EXIT_FAILURE;
    }

    bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
    write(fd, buffer, bytes_written);
    close(fd);

    return EXIT_SUCCESS;
}


static int gpio_unexport(int pin)
{
    char buffer[BUFFER_MAX];
    ssize_t bytes_written;
    int fd;

    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open unexport for writing.\n");
        return EXIT_FAILURE;
    }

    bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
    write(fd, buffer, bytes_written);
    close(fd);

    return EXIT_SUCCESS;
}


static int gpio_direction(int pin, int dir)
{
    static const char s_directions_str[] = "in\0Out";
    char path[DIRECTION_MAX];
    int fd;

    snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
    fd = open(path, O_WRONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open gpio direction for writing.\n");
        return EXIT_FAILURE;
    }

    if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
        fprintf(stderr, "Failed to set direction.\n");
        return EXIT_FAILURE;
    }

    close(fd);

    return EXIT_FAILURE;
}


static int gpio_read(int pin)
{
    char path[VALUE_MAX];
    char value_str[3];
    int fd;

    snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_RDONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open gpio value for reading.\n");
        return EXIT_FAILURE;
    }

    if (-1 == read(fd, value_str, 3)) {
        fprintf(stderr, "Failed to read value.\n");
        return EXIT_FAILURE;
    }

    close(fd);

    return(atoi(value_str));
}


static int gpio_write(int pin, int value)
{
    static const char s_values_str[] = "01";
    char path[VALUE_MAX];
    int fd;

    snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_WRONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open gpio value for writing.\n");
        return EXIT_FAILURE;
    }

    if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
        fprintf(stderr, "Failed to write value.\n");
        return EXIT_FAILURE;
    }

    close(fd);

    return EXIT_SUCCESS;
}
