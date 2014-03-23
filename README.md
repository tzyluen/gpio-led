gpio-led
=======================================================
Raspberry Pi’s GPIO, a simple LED controller.  Reference to <cite>[Pi's GPIO pins][1]</cite>


Demo
----
GPIO wiring:
![alt tag](https://raw.github.com/tzyluen/gpio-led/master/img/gpio-led-3.jpeg)

Single LED blinks:                                                             | Eight LEDs blink:
:-----------------------------------------------------------------------------:|:-----------------------------------------------------------------------------:
![alt tag](https://raw.github.com/tzyluen/gpio-led/master/img/gpio-led-1.jpeg) | ![alt tag](https://raw.github.com/tzyluen/gpio-led/master/img/gpio-led-2.jpeg)


Notes
-----
#####Linux kernel modules:
After a few `rpi-update`, the kernel source/header version will diverged and will get 'Invalid module format' during `insmod`:

    Error: could not insert module ./kgpio_led.ko: Invalid module format

The kernel source and current running kernel needs to be exact same version.

1. Use `modinfo` to identify the compiled modules kernel version:

        filename:       /home/pi/projects/gpio-led/modules/kgpio_led.ko
        license:        GPL
        srcversion:     7572006FA49CEA8DE7D93A9
        depends:        
        vermagic:       3.6.11 preempt mod_unload modversions ARMv6 

3. If the current running kernel version ain't same, either 1) rebuild the kernel or 2) fix the issues without rebuild the kernel. Below is without rebuild the kernel:

        cd ~/projects
        git clone https://github.com/raspberrypi/linux.git
        mv linux linux-rpi-3.12.y
        ln -s linux-rpi-3.12.y /usr/src/linux-rpi-3.12.y
        ln -s /usr/src/linux-rpi-3.12.y /lib/modules/$(uname -r)/build
        cd /lib/modules/$(uname -r)/build
        make mrproper
        gzip -dc /proc/config.gz > .config
        make modules_prepare
        wget https://github.com/raspberrypi/firmware/raw/master/extra/Module.symvers


References
----------
[1]:http://pi4j.com/images/p1header.png
