#ifndef GPIO_LED_H_
#define GPIO_LED_H_

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

/* GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y) */
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  /* sets bits which are 1 ignores bits which are 0 */
#define GPIO_CLR *(gpio+10) /* clears bits which are 1 ignores bits which are 0 */

/**
 * Global variables
 */
int  mem_fd;                /* /dev/mem fd */
void *gpio_map;             /* */
volatile unsigned *gpio;    /* I/O access */

void setup_gpio();

#endif
