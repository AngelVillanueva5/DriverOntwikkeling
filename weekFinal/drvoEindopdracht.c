#include <linux/proc_fs.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/mod_devicetable.h>
#include <linux/of.h>
#include <linux/ioport.h>
#include <linux/i2c.h>

MODULE_LICENSE("GPL");
static struct i2c_client *i2c_client;
static struct class *i2c_class;
static struct device *i2c_device;
static struct platform_device *adc_pdev;
static int i2c_remove(struct platform_device *pdev);
static int i2c_probe(struct platform_device *pdev);
static ssize_t show(struct device *dev, struct device_attribute *attr, char *buf);
static ssize_t store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);

static const struct of_device_id my_driver_ids[] = {
    { .compatible = "drvoi2c" },
    { }
};

static struct platform_driver my_driver = {
    .probe = i2c_probe,
    .remove = i2c_remove,
    .driver = {
        .name = "drvoi2c",
        .of_match_table = of_match_ptr(my_driver_ids),
    },
};

struct i2c_members {
    uint8_t reg;
};



static struct device_attribute dev_attr_register_value = {
    .attr = {
        .name = "value",
        .mode = 0666,
    },
    .show = show,
    .store = store
};

struct my_platform_data {
    struct i2c_board_info i2c_board_info;
};

static ssize_t store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
     printk("STORE START");
    long val;
    if (kstrtol(buf, 0, &val) == 0) {
        // MASTER WRITE TO SLAVE
        i2c_smbus_write_byte(i2c_client, (u8)val);
    }
    printk("STORE END");
    return count;
}

static ssize_t show(struct device *dev, struct device_attribute *attr, char *buf) {
     printk("SHOW START");
    char receiveVal[3];
    // MASTER RECEIVE
    int ret = i2c_master_recv(i2c_client, receiveVal, sizeof(receiveVal));
    printk("SHOW END");
    return snprintf(buf, PAGE_SIZE, "%d\n", receiveVal[0]);
}

static int i2c_probe(struct platform_device *pdev) {
     printk("PROBE START");
    struct device_node *np = pdev->dev.of_node;
    struct i2c_adapter *adapter;
    struct i2c_board_info info;
    int reg_value, i2c_bus;
    int ret;

    if (!np) {
        dev_err(&pdev->dev, "No device tree node\n");
        return -ENODEV;
    }

    // READ REG AND BUS FROM TREE
    ret = of_property_read_u32(np, "reg", &reg_value);
    ret = of_property_read_u32(np, "bus", &i2c_bus);
    adapter = i2c_get_adapter(i2c_bus);
    memset(&info, 0, sizeof(struct i2c_board_info));
    strlcpy(info.type, "custom_sensor", I2C_NAME_SIZE);
    info.addr = reg_value;

    i2c_client = i2c_new_device(adapter, &info);
    
    printk("start device crate");
    i2c_device = device_create(i2c_class, NULL, MKDEV(0, 0), NULL, "drvoi2c%d", 0);
	printk("end device create");
    if (device_create_file(i2c_device, &dev_attr_register_value)) {
        device_destroy(i2c_class, MKDEV(0, 0));
      
        i2c_unregister_device(i2c_client);
        i2c_put_adapter(adapter);
        return -ENOMEM;
    }

    i2c_put_adapter(adapter);
    printk("PROBE END");
    return 0;
}

static int i2c_remove(struct platform_device *pdev) {
     printk("REMOVE START");
    struct i2c_members *data = dev_get_drvdata(&pdev->dev);
    device_remove_file(i2c_device, &dev_attr_register_value);
    device_destroy(i2c_class, MKDEV(0, 0));
    kfree(data);
    i2c_unregister_device(i2c_client);
     printk("REMOVE END");
    return 0;
}

static void i2c_exit(void) {
    printk("EXIT START");
    platform_driver_unregister(&my_driver);
    if (i2c_device) {
        device_destroy(i2c_class, MKDEV(0, 0));
        i2c_device = NULL;
    }
    device_remove_file(i2c_device, &dev_attr_register_value);
    if (i2c_class) {
        class_destroy(i2c_class);
        i2c_class = NULL;
    }
     printk("EXIT END");
}

static int i2c_init(void) {
    printk("INIT START");
    int result;
    i2c_class = class_create(THIS_MODULE, "drvoi2c");
    if (IS_ERR(i2c_class)) {
        printk("IS ERROR");
        return PTR_ERR(i2c_class);
    }

    result = platform_driver_register(&my_driver);
    if (result) {
        class_destroy(i2c_class);
    }
    printk("INIT END");
    return result;
}

module_init(i2c_init);
module_exit(i2c_exit);
