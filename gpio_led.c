/**
 * References:
 * 1) http://elinux.org/Rpi_Low-level_peripherals#C_2
 * 2) http://startingelectronics.com/software/raspberry-PI/raspberry-PI-flash-LED-C
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "gpio_led.h"


int main(int argc, char **argv)
{
    setup_gpio();  /* Set up gpio pointer for direct register access */

    int p[] = {11, 12, 13, 15, 16, 18, 22, 7};
    // P1: 12 <-> 18 ?
    // P3: 15
    
    int i;
    for (i = 0; i < sizeof(p); ++i) {
        printf("setting: pin %d\n", p[i]);
        INP_GPIO(p[i]); /* use INP_GPIO(x) before we can use OUT_GPIO(x) */
        OUT_GPIO(p[i]); /* set GPIO pin 7 as output */
        GPIO_SET = (1 << p[i]);
        sleep(1);
        GPIO_CLR = (1 << p[i]);

       // /* flash LED on and off 10 times */
       // int i;  /* repeat count */
       // for (i = 0; i < 10; ++i) {
       //     GPIO_SET = (1 << p);
       //     sleep(1);
       //     GPIO_CLR = (1 << p);
       //     sleep(1);
       // }
    }

    return EXIT_SUCCESS;
}


/**
 * Set up a memory regions to access GPIO
 */
void setup_gpio()
{
    /* open /dev/mem */
    if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
        printf("can't open /dev/mem \n");
        exit(-EXIT_FAILURE);
    }

    /* mmap GPIO */
    gpio_map = mmap(
          NULL,                 /* Any adddress in our space will do */
          BLOCK_SIZE,           /* Map length */
          PROT_READ|PROT_WRITE, /* Enable reading & writting to mapped memory */
          MAP_SHARED,           /* Shared with other processes */
          mem_fd,               /* File to map */
          GPIO_BASE             /* Offset to GPIO peripheral */
    );

    close(mem_fd);

    if (gpio_map == MAP_FAILED) {
          printf("mmap error %d\n", (int) gpio_map);
          exit(-EXIT_FAILURE);
    }

    gpio = (volatile unsigned *)gpio_map;
}
