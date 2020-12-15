#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
MODULE_AUTHOR("Kataoka Tatsumi");
MODULE_DESCRIPTION("driver for Stepping motor [SM-42BYG011]");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;
static volatile u32 *gpio_base = NULL;
static int gpio_pins[] = {5, 6, 13, 19};

static ssize_t motor_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
    char c;

    if(copy_from_user(&c,buf,sizeof(char)))
        return -EFAULT;

    /*memo
     * gpio
        * 10 is off
        * 7 is on
    */
    //1 ~ 4は低トルク
    if (c == '1') {
        gpio_base[7] = 1 << gpio_pins[0];
        gpio_base[10] = 1 << gpio_pins[1];
        gpio_base[10] = 1 << gpio_pins[2];
        gpio_base[10] = 1 << gpio_pins[3];
    }
    else if(c == '2') {
        gpio_base[10] = 1 << gpio_pins[0];
        gpio_base[10] = 1 << gpio_pins[1];
        gpio_base[7] = 1 << gpio_pins[2];
        gpio_base[10] = 1 << gpio_pins[3];
    }
    else if (c == '3') {
        gpio_base[10] = 1 << gpio_pins[0];
        gpio_base[7] = 1 << gpio_pins[1];
        gpio_base[10] = 1 << gpio_pins[2];
        gpio_base[10] = 1 << gpio_pins[3];
    }
    else if (c == '4') {
        gpio_base[10] = 1 << gpio_pins[0];
        gpio_base[10] = 1 << gpio_pins[1];
        gpio_base[10] = 1 << gpio_pins[2];
        gpio_base[7] = 1 << gpio_pins[3];
    }
    // ==== 1相はここまで ====
    // 2相はここから
    else if (c == '5') {
        gpio_base[7] = 1 << gpio_pins[0];
        gpio_base[10] = 1 << gpio_pins[1];
        gpio_base[7] = 1 << gpio_pins[2];
        gpio_base[10] = 1 << gpio_pins[3];
    }
    else if (c == '6') {
        gpio_base[10] = 1 << gpio_pins[0];
        gpio_base[7] = 1 << gpio_pins[1];
        gpio_base[7] = 1 << gpio_pins[2];
        gpio_base[10] = 1 << gpio_pins[3];
    }
    else if (c == '7') {
        gpio_base[10] = 1 << gpio_pins[0];
        gpio_base[7] = 1 << gpio_pins[1];
        gpio_base[10] = 1 << gpio_pins[2];
        gpio_base[7] = 1 << gpio_pins[3];
    }
    else if (c == '8') {
        gpio_base[7] = 1 << gpio_pins[0];
        gpio_base[10] = 1 << gpio_pins[1];
        gpio_base[10] = 1 << gpio_pins[2];
        gpio_base[7] = 1 << gpio_pins[3];
    }
    // ==== 2相はここまで ====
    // 停止
    else if (c == '0') {
        gpio_base[10] = 1 << gpio_pins[0];
        gpio_base[10] = 1 << gpio_pins[1];
        gpio_base[10] = 1 << gpio_pins[2];
        gpio_base[10] = 1 << gpio_pins[3];
    }
    return 1;
}

static struct file_operations motor_fops = {
        .owner = THIS_MODULE,
        .write = motor_write
};

static int __init init_mod(void) //カーネルモジュールの初期化
{
    int retval;
    unsigned int i;

    retval =  alloc_chrdev_region(&dev, 0, 1, "mystep");
    if(retval < 0){
        printk(KERN_ERR "alloc_chrdev_region failed.\n");
        return retval;
    }
    cdev_init(&cdv, &motor_fops);
    retval = cdev_add(&cdv, dev, 1);
    if(retval < 0){
        printk(KERN_ERR "cdev_add failed. major:%d, minor:%d",MAJOR(dev),MINOR(dev));
        return retval;
    }
    cls = class_create(THIS_MODULE,"mystep");   //ここから追加
    if(IS_ERR(cls)){
        printk(KERN_ERR "class_create failed.");
        return PTR_ERR(cls);
    }
    device_create(cls, NULL, dev, NULL, "mystep%d",MINOR(dev));
    printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__,MAJOR(dev));
    gpio_base = ioremap_nocache(0x3f200000, 0xA0);
    for (i=0; i<4; i++)
    {
        const u32 motor = gpio_pins[i];
        const u32 index = motor/10;//GPFSEL2
        const u32 shift = (motor%10)*3;//15bit
        const u32 mask = ~(0x7 << shift);
        gpio_base[index] = (gpio_base[index] & mask) | (0x1 << shift);
    }
    return 0;
}

static void __exit cleanup_mod(void) //後始末
{
    cdev_del(&cdv);
    device_destroy(cls, dev);
    class_destroy(cls);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "%s is unloaded. major:%d\n",__FILE__,MAJOR(dev));
    //printk(KERN_INFO "%s is unloaded.\n",__FILE__);
}

module_init(init_mod);     // マクロで関数を登録
module_exit(cleanup_mod);  // 同上
