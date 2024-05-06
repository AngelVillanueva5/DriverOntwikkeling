#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/io.h>

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

 /* PIN */
#define PIN 18

/* base address */
#define GPIO1_ADDR 0x4804c000

/* register offsets in uint32_t sizes */
#define GPIO_OE 0x4D // 0x134
#define GPIO_DATAIN 0x4E // 0x138
#define GPIO_CLEARDATAOUT 0x64 // 0x190
#define GPIO_SETDATAOUT 0x65 // 0x194

/* max size in bytes */
#define GPIO_MAX 0x198

uint32_t* gpio1;
uint32_t oe;

void setPinMode(void){
/* output instellen */
	gpio1 = ioremap( GPIO1_ADDR, GPIO_MAX * sizeof(uint32_t) );
	barrier();
	oe = ioread32( gpio1 + GPIO_OE );
	rmb();
	iowrite32( (oe & (~(1<<PIN))), gpio1 + GPIO_OE );
	wmb(); // write memory barrier
	iounmap(gpio1);
}

void setLed(bool state){
	if (state){
		/* ledje aan en uit zetten */
		gpio1 = ioremap(GPIO1_ADDR, GPIO_MAX);
		barrier();
		iowrite32( (1<<PIN), gpio1 + GPIO_SETDATAOUT ); // Pin 18 aan
		wmb();
		iounmap(gpio1);
		printk(KERN_INFO "ledstatus: %d\n", state);
	} else {
		gpio1 = ioremap(GPIO1_ADDR, GPIO_MAX);
		barrier();
		iowrite32((1<<PIN), gpio1 + GPIO_CLEARDATAOUT);
		wmb();
		iounmap(gpio1);
		printk(KERN_INFO "ledstatus: %d\n", state);
	}
}

void toggleLed(void)
{
	static bool ledStatus = false;
	setLed(ledStatus);
	ledStatus = !ledStatus;
}

bool getLedStatus(void) 
{
	uint32_t ledStatus = 0;
	gpio1 = ioremap(GPIO1_ADDR, GPIO_MAX * sizeof(uint32_t));
	barrier();
	ledStatus = ioread32(gpio1 + GPIO_DATAIN);
	rmb();
	iounmap(gpio1);
	printk(KERN_INFO "ledstatus: %d\n", (ledStatus & (1<<PIN)));
	return (ledStatus & (1<<PIN)) << PIN;
}

static ssize_t hello_read(struct file* file, char __user* buf, size_t lbuf, loff_t* ppos) {
	getLedStatus();
	return 0;
}

static ssize_t hello_write(struct file* file, const char __user* buf, size_t lbuf, loff_t* ppos) {
	    printk(KERN_ALERT "hello_write()\n");
	    static bool ledStatus = false;
	    setLed(ledStatus);
   	    ledStatus = !ledStatus;

	    return lbuf;
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


struct file_operations fops = {
	.read = hello_read,
	.write = hello_write,
	.open = hello_open,
	.release = hello_release,
};

static int hello_init(void) {
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
	setPinMode();
	return result;
}

static void hello_exit(void) {
	dev_t device_number;
	printk(KERN_ALERT "hello_exit()\n");
	device_number = MKDEV(major, minor);
	unregister_chrdev_region(device_number, amount);
}

module_init(hello_init);
module_exit(hello_exit);
