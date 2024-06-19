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

MODULE_DESCRIPTION("A driver for my simple AMTEGA I2C ADC");

static struct i2c_client *i2c_client;
static struct class *i2c_custom_class;
static struct device *i2c_custom_device;

static int i2c_custom_remove(struct i2c_client *client);
static int i2c_custom_probe(struct i2c_client *client, const struct i2c_device_id *id);
static ssize_t my_show(struct device *dev, struct device_attribute *attr, char *buf);
static ssize_t my_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);

static const struct of_device_id my_driver_ids[] = {
    { .compatible = "drvoi2c" },
    { }
};

static const struct i2c_device_id i2c_ids[] = {
    { "drvoi2c", 0 },
    { }
};

struct i2c_driver my_driver = {
    .probe = i2c_custom_probe,
    .remove = i2c_custom_remove,
    .driver = {
        .name = "drvoi2c",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(my_driver_ids),
    },
    .id_table = i2c_ids,
};

MODULE_DEVICE_TABLE(i2c, i2c_ids);

struct i2c_members {
    uint8_t reg;
};

static struct i2c_members *i2c_data;

static struct device_attribute dev_attr_register_value = {
    .attr = {
        .name = "register-value",
        .mode = 0666,
    },
    .show = my_show,
    .store = my_store
};

static ssize_t my_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
    printk(KERN_ALERT "store action");
    long val;
    if (kstrtol(buf, 0, &val) == 0) {
        i2c_smbus_write_byte(i2c_client, (u8)val);
    }
    return count;
}

static ssize_t my_show(struct device *dev, struct device_attribute *attr, char *buf) {
    printk(KERN_ALERT "show action");
    char recv[3];
    int ret = i2c_master_recv(i2c_client, recv, sizeof(recv));
    if (ret < 0) return ret; // return error

    return snprintf(buf, PAGE_SIZE, "%d\n", recv[0]);
}

static int i2c_custom_probe(struct i2c_client *client, const struct i2c_device_id *id) {
    struct device *dev = &client->dev;
    int ret;

    i2c_client = client;

    i2c_data = kzalloc(sizeof(struct i2c_members), GFP_KERNEL);
    if (!i2c_data) {
        dev_err(dev, "Failed to allocate memory\n");
        return -ENOMEM;
    }

    dev_set_drvdata(dev, i2c_data);

    i2c_custom_device = device_create(i2c_custom_class, NULL, MKDEV(0, 0), NULL, "I2C device%d", 0);
    if (IS_ERR(i2c_custom_device)) {
        ret = PTR_ERR(i2c_custom_device);
        kfree(i2c_data);
        return ret;
    }

    ret = device_create_file(i2c_custom_device, &dev_attr_register_value);
    if (ret) {
        device_destroy(i2c_custom_class, MKDEV(0, 0));
        kfree(i2c_data);
        return ret;
    }

    return 0;
}

static int i2c_custom_remove(struct i2c_client *client) {
    device_remove_file(i2c_custom_device, &dev_attr_register_value);
    device_destroy(i2c_custom_class, MKDEV(0, 0));
    kfree(i2c_data);
    return 0;
}

static void i2c_custom_exit(void) {
    class_destroy(i2c_custom_class);
    //i2c_del_driver(&my_driver);
}

static int i2c_custom_init(void) {
    int result;

    i2c_custom_class = class_create(THIS_MODULE, "drvoi2c");
    if (IS_ERR(i2c_custom_class)) {
        return PTR_ERR(i2c_custom_class);
    }

    result = i2c_add_driver(&my_driver);
    if (result) {
        class_destroy(i2c_custom_class);
    }
    return result;
}

module_init(i2c_custom_init);
module_exit(i2c_custom_exit);