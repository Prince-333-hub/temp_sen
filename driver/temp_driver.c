#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/random.h>
#include <linux/device.h>

#define DEVICE_NAME "temp_sensor"
#define CLASS_NAME "temp"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jyotiraditya Pati");
MODULE_DESCRIPTION("Simulated Linux Character Device Driver for IoT Temperature Sensor");
MODULE_VERSION("1.0");

static int majorNumber;
static struct class* tempClass = NULL;
static struct device* tempDevice = NULL;

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);

static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .release = dev_release,
};

static int __init temp_init(void) {
    printk(KERN_INFO "TempDriver: Initializing kernel module\n");
    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0) {
        printk(KERN_ALERT "TempDriver failed to register major number\n");
        return majorNumber;
    }

    tempClass = class_create(CLASS_NAME);
    if (IS_ERR(tempClass)) {
        unregister_chrdev(majorNumber, DEVICE_NAME);
        return PTR_ERR(tempClass);
    }

    tempDevice = device_create(tempClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(tempDevice)) {
        class_destroy(tempClass);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        return PTR_ERR(tempDevice);
    }

    printk(KERN_INFO "TempDriver: Device registered at /dev/%s\n", DEVICE_NAME);
    return 0;
}

static void __exit temp_exit(void) {
    device_destroy(tempClass, MKDEV(majorNumber, 0));
    class_destroy(tempClass);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_INFO "TempDriver: Goodbye from kernel module\n");
}

static int dev_open(struct inode *inodep, struct file *filep) {
    return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    char temp_str[16];
    unsigned int rand_val;
    get_random_bytes(&rand_val, sizeof(rand_val));
    
    // Generate simulated integer values between 2000 and 8000 (representing 20.00 to 80.00 °C)
    int temp_int = 2000 + (rand_val % 6001); 
    int whole = temp_int / 100;
    int frac = temp_int % 100;

    snprintf(temp_str, sizeof(temp_str), "%d.%02d\n", whole, frac);

    int str_len = strlen(temp_str);
    if (copy_to_user(buffer, temp_str, str_len)) {
        return -EFAULT;
    }
    return str_len;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    return 0;
}

module_init(temp_init);
module_exit(temp_exit);
