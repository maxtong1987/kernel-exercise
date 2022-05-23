#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/ioport.h>
#include <linux/io.h>

#define DRIVER_NAME "drvled"

#define LED_OFF         0
#define LED_ON          1

#define GPIO1_BASE      0x3F000000 //raspberry pi 4 only
#define GPIO1_SIZE      8

#define GPIO1_REG_DATA  0
#define GPIO1_REG_DIR   4

#define GPIO_BIT        (1 << 5)

static struct {
    dev_t devnum;
    struct cdev cdev;
    unsigned int led_status;
    void __iomem *regbase;
} drvled_data;

static void drvled_setled(unsigned int status) {
    u32 val;

    /* set value */
    val = readl(drvled_data.regbase + GPIO1_REG_DATA);
    if (status == LED_ON) 
        val |= GPIO_BIT;
    else if (status == LED_OFF)
        val &= ~GPIO_BIT;
    writel(val, drvled_data.regbase + GPIO1_REG_DATA);

    /* update status */
    drvled_data.led_status = status;
}

static void drvled_setdirection(void) {
    u32 val;

    val = readl(drvled_data.regbase + GPIO1_REG_DIR);
    val |= GPIO_BIT;
    writel(val, drvled_data.regbase + GPIO1_REG_DIR);
}

static ssize_t drvled_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    static const char * const msg[] = { "OFF\n", "ON\n" };
    int size;

    /* check if EOF */
    if (*ppos > 0)
        return 0;

    size = strlen(msg[drvled_data.led_status]);
    if (size > count)
        size = count;

    if (copy_to_user(buf, msg[drvled_data.led_status], size))
        return -EFAULT;

    *ppos += size;

    return size;
}

static ssize_t drvled_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
    char kbuf = 0;

    if(copy_from_user(&kbuf, buf, 1))
        return -EFAULT;
    
    if(kbuf == '1') {
        drvled_setled(LED_ON);
        pr_info("LED ON!\n");
    } else if (kbuf == '0') {
        drvled_setled(LED_OFF);
        pr_info("LED OFF!\n");
    }

    return count;
}

static const struct file_operations drvled_fops = {
    .owner = THIS_MODULE,
    .write = drvled_write,
    .read = drvled_read,
};

static int __init drvled_init(void) {
    int result;

    if(!request_mem_region(GPIO1_BASE, GPIO1_SIZE, DRIVER_NAME)) {
        pr_err("%s: Error requesting I/O!\n", DRIVER_NAME);
        result = -EBUSY;
        goto ret_err_request_mem_region;
    }

    drvled_data.regbase = ioremap(GPIO1_BASE, GPIO1_SIZE);
    if(!drvled_data.regbase) {
        pr_err("%s: Error mapping I/O!\n", DRIVER_NAME);
        result = -ENOMEM;
        goto err_ioremap;
    }

    result = alloc_chrdev_region(&drvled_data.devnum, 0, 1, DRIVER_NAME);
    if(result) {
        pr_err("%s: failed to allocate device number!\n", DRIVER_NAME);
        goto ret_err_alloc_chrdev_region;
    }

    cdev_init(&drvled_data.cdev, &drvled_fops);

    result = cdev_add(&drvled_data.cdev, drvled_data.devnum, 1);
    if(result) {
        pr_err("%s: Char device registration failed!\n", DRIVER_NAME);
        // unregister_chrdev_region(drvled_data.devnum, 1);
        // return result;
        goto ret_err_cdev_add;
    }

    drvled_setdirection();

    drvled_setled(LED_OFF);

    pr_info("%s: initalized.\n", DRIVER_NAME);
    goto ret_ok;

ret_err_cdev_add:
    unregister_chrdev_region(drvled_data.devnum, 1);
ret_err_alloc_chrdev_region:
    iounmap(drvled_data.regbase);
err_ioremap:
    release_mem_region(GPIO1_BASE, GPIO1_SIZE);
ret_err_request_mem_region:
ret_ok:
    return result;
}

static void __exit drvled_exit(void) {
    cdev_del(&drvled_data.cdev);
    unregister_chrdev_region(drvled_data.devnum, 1);
    pr_info("%s: exitting.\n", DRIVER_NAME);
}

module_init(drvled_init);
module_exit(drvled_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Max Tong <maxtong198776@gmail.com>");
MODULE_VERSION("1.0");
