#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

MODULE_LICENSE("Dual BSD/GPL");

static const int major = 500;
static const int minor = 1;
static const int amount = 0; // amount of major nrs.
static const char driver_name[] = "hello_driver";
/* device structures */
static struct cdev* device;
static int param = 0;
module_param_named(param, param, int, 0644);
extern struct file_operations fops;

static int hello_init(void)
{
	dev_t device_number;
	int result;
	printk(KERN_ALERT "hello init\n");
	device_number = MKDEV(major, minor);
	device = cdev_alloc();
	cdev_init(device, &fops);
	result = register_chrdev_region(device_number, amount, driver_name);
	if (result != 0)
	{
		printk(KERN_ALERT "hello init failed");
	}
	result = cdev_add(device, device_number, minor);
	printk(KERN_INFO "major:%d minor:%d", major, minor );

	return result;
}

static void hello_exit(void)
{
	dev_t device_number;
	printk(KERN_ALERT "hello_exit()\n");
	device_number = MKDEV(major, minor);
	unregister_chrdev_region(device_number, amount);
}

static int hello_open(struct inode *inode, struct file *file)
{
	printk(KERN_ALERT "hello_open()\n");
	return 0;
}
static int hello_release(struct inode *inode, struct file *file)
{
	printk(KERN_ALERT "hello_release()\n");
	return 0;
}

/**
* read / write
**/
static ssize_t hello_read(struct file *file, char __user * buf, size_t lbuf, loff_t * pos)
{
	printk(KERN_ALERT "hello_read()\n");
	printk(KERN_INFO "%zu:lbuf", lbuf);
	return 0;
}

static ssize_t hello_write(struct file *file, const char __user * buf, size_t lbuf, loff_t * ppos)
{
	printk(KERN_ALERT "hello_write())\n");
	printk(KERN_INFO "%zu:lbuf", lbuf);

	return lbuf;
}

struct file_operations fops = {
	.read = hello_read,
	.write = hello_write,
	.open = hello_open,
	.release = hello_release,
};

module_init(hello_init);
module_exit(hello_exit);
