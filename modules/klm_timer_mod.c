#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");

//static struct timer_list klm_timer;

void klm_timer_callback(unsigned long data)
{
    printk("klm_timer_callback jiffies: %ld\n", jiffies);
}


int klm_timer_init(void)
{
    //int ret;
    
    printk("(%d) %s loaded\n", __LINE__, THIS_MODULE->name);
    //setup_timer(&klm_timer, klm_timer_callback, 0);
    //ret = mod_timer(&klm_timer, jiffies + msecs_to_jiffies(200));

    //if (ret)
    //    printk("Error: (%d) mod_timer\n", __LINE__);
    return 0;
}


void klm_timer_exit(void)
{
    //int ret;

    //ret = del_timer(&klm_timer);
    //if (ret)
        printk("Error: (%d) del_timer\n", __LINE__);

    printk("(%d) %s unloaded\n", __LINE__, THIS_MODULE->name);
}


module_init(klm_timer_init);
module_exit(klm_timer_exit);
