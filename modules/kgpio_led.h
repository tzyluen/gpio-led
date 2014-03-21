/**
 * GPIO pins can be controlled by writing to GPFSELx and GPFCLRs registers.
 * Structure `t_gpio_registers' served such purpose.
 */
typedef struct {
    uint32_t GPFSEL[6];
    uint32_t reserved1;
    uint32_t GPSET[2];
    uint32_t reserved2;
    uint32_t GPCLR[2];
} t_gpio_registers;
t_gpio_registers *sp_gpio_registers;

static struct timer_list s_blink_timer;
static int s_blink_period = 1000;
static const int led_gpio_pin = 18;
static struct class *sp_device_class;
static struct device *sp_device_object;


static void set_gpio_function(int, int);
static void set_gpio_output_value(int, bool);
static void blink_timer_handler(unsigned long unused);
static ssize_t set_period_callback(struct device *, struct device_attribute *, const char *, size_t);
