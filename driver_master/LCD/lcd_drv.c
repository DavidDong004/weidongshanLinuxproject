#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/fb.h>
#include <linux/init.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/cpufreq.h>
#include <linux/io.h>

#include <asm/div64.h>

#include <asm/mach/map.h>
#include <mach/regs-lcd.h>
#include <mach/regs-gpio.h>
#include <mach/fb.h>

static struct fb_info *g_ptMyFb_info;

/* 1.Entrance */

int __init lcd_drv_init(void)
{
    /* 1.1 assignment fb_info*/
    g_ptMyFb_info = framebuffer_alloc(0, NULL);

    /* 1.2 set fb_info*/

    /* a.var */
    g_ptMyFb_info->var.xres = 1024;
    g_ptMyFb_info->var.yres = 600;
    g_ptMyFb_info->var.bit_per_pixel = 16; /* RGB565 */
    g_ptMyFb_info->red.offset = 11;
    g_ptMyFb_info->red.length = 5;
    g_ptMyFb_info->green.offset = 11;
    g_ptMyFb_info->green.length = 5;
    g_ptMyFb_info->blue.offset = 11;
    g_ptMyFb_info->blue.length = 5;

    /* b.fix */

    /* c.fb_ops */

    /* 1.3 register fb_info*/

	return 0;
}

/* 2,Exit */

static void __exit lcd_drv_exit(void)
{
    /*2.1 unregister fb_info  */

    /* 2.2 release fb_info*/
	platform_driver_unregister(&s3c2410fb_driver);
	platform_driver_unregister(&s3c2412fb_driver);
}

module_init(lcd_drv_init);
module_exit(lcd_drv_exit);

MODULE_AUTHOR("www.100ask.net");
MODULE_DESCRIPTION("Framebuffer driver for the linux");
MODULE_LICENSE("GPL");

