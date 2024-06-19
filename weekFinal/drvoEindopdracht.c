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
static struct class *class_i2c;
static struct device *i2c_device;
static struct platform_device *adc_pdev;
static int remove_i2c(struct platform_device *pdev);
static int probe_i2c(struct platform_device *pdev);
static ssize_t show(struct device *dev, struct device_attribute *attr, char *buf);
static ssize_t store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);

static const struct of_device_id my_driver_ids[] = {
    { .compatible = "drvoi2c" },
    { }
};

static struct platform_driver my_driver = {
    .probe = probe_i2c,
    .remove = remove_i2c,
    .driver = {
        .name = "drvoi2c",
        .of_match_table = of_match_ptr(my_driver_ids),
    },
};

struct i2c_members {
    uint8_t reg;
};

struct my_platform_data {
    struct i2c_board_info i2c_board_info;
};

static struct i2c_members *i2c_data;

static struct device_attribute dev_attr_register_value = {
    .attr = {
        .name = "value",
        .mode = 0666,
    },
    .show = show,
    .store = store
};

static ssize_t store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
    printk("Start of store\n");
    long val;
    if (kstrtol(buf, 0, &val) == 0) {
        i2c_smbus_write_byte(i2c_client, (u8)val);
    }
    return count;
}

static ssize_t show(struct device *dev, struct device_attribute *attr, char *buf) {
    
    char recv[3];
    int ret = i2c_master_recv(i2c_client, recv, sizeof(recv));

    return snprintf(buf, PAGE_SIZE, "%d\n", recv[0]);
}



static int probe_i2c(struct platform_device *pdev) {
    struct device_node *np = pdev->dev.of_node;
    struct i2c_adapter *adapter;
    struct i2c_board_info info;
    int reg_value, i2c_bus;
    int ret;

    if (!np) {
        dev_err(&pdev->dev, "No device tree node\n");
        return -ENODEV;
    }

    ret = of_property_read_u32(np, "reg", &reg_value);
    ret = of_property_read_u32(np, "i2c-bus", &i2c_bus);
    adapter = i2c_get_adapter(i2c_bus);
    memset(&info, 0, sizeof(struct i2c_board_info));
    strlcpy(info.type, "custom_sensor", I2C_NAME_SIZE);
    info.addr = reg_value;

    i2c_client = i2c_new_device(adapter, &info);

    i2c_data = kzalloc(sizeof(struct i2c_members), GFP_KERNEL);
    dev_set_drvdata(&pdev->dev, i2c_data);
    i2c_device = device_create(class_i2c, NULL, MKDEV(0, 0), NULL, "I2C_device%d", 0);

    if (device_create_file(i2c_device, &dev_attr_register_value)) {
        device_destroy(class_i2c, MKDEV(0, 0));
        kfree(i2c_data);
        i2c_unregister_device(i2c_client);
        i2c_put_adapter(adapter);
        return -ENOMEM;
    }

    i2c_put_adapter(adapter);
    return 0;
}

static int remove_i2c(struct platform_device *pdev) {
    struct i2c_members *data = dev_get_drvdata(&pdev->dev);
    device_remove_file(i2c_device, &dev_attr_register_value);
    device_destroy(class_i2c, MKDEV(0, 0));
    kfree(data);
    i2c_unregister_device(i2c_client);

    return 0;
}
static void exit_i2c(void) {
    platform_driver_unregister(&my_driver);
    device_destroy(class_i2c, MKDEV(0, 0));
    i2c_device = NULL;

    device_remove_file(i2c_device, &dev_attr_register_value);
    class_destroy(class_i2c);
    class_i2c = NULL;
}

static int init_i2c(void) {
    int result;
    class_i2c = class_create(THIS_MODULE, "drvoi2c");

    result = platform_driver_register(&my_driver);

    if (result) {
        class_destroy(class_i2c);
    }

    return result;
}

module_init(init_i2c);
module_exit(exit_i2c);
