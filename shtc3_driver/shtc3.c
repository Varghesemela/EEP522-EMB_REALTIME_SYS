/*
 * shtc3.c - Basic I²C driver for the Sensirion SHTC3 sensor with character device support.
 *
 * This driver wakes the sensor, triggers a measurement, reads the data, checks the CRC,
 * converts the values to physical units, and exposes temperature and humidity via sysfs
 * as well as via a character device (e.g. /dev/shtc3 or /dev/shtc3_dome).
 *
 * Note: Command values (wake, sleep, measure) are taken from the datasheet.
 */

 #include <linux/module.h>
 #include <linux/i2c.h>
 #include <linux/delay.h>
 #include <linux/mutex.h>
 #include <linux/slab.h>
 #include <linux/sysfs.h>
 #include <linux/kernel.h>
 #include <linux/fs.h>
 #include <linux/cdev.h>
 #include <linux/device.h>
 #include <linux/uaccess.h>
 
 /* Command definitions (verify with your datasheet) */
 #define SHTC3_CMD_WAKEUP   0x3517
 #define SHTC3_CMD_SLEEP    0xB098
 #define SHTC3_CMD_MEASURE  0x7CA2
 
 static struct i2c_driver shtc3_driver;

 /* Driver private data structure */
 struct shtc3_data {
     struct i2c_client *client;
     struct mutex lock;
     /* Character device fields */
     dev_t devt;
     struct cdev cdev;
     struct device *device;
 };
 
 #ifdef CONFIG_OF
 static const struct of_device_id shtc3_of_match[] = {
     { .compatible = "sensirion,shtc3", },
     { },
 };
 MODULE_DEVICE_TABLE(of, shtc3_of_match);
 #endif
 
 static const struct i2c_device_id shtc3_id[] = {
     { "shtc3", 0 },
     { "shtc3_dome", 0 },
     { }
 };
 MODULE_DEVICE_TABLE(i2c, shtc3_id);
 
 /* Global class pointer; created once in module init */
 static struct class *shtc3_class;
 
 /* CRC8 calculation with polynomial 0x31 (x^8 + x^5 + x^4 + 1) */
 static u8 shtc3_crc8(const u8 *data, int len)
 {
     u8 crc = 0xFF;
     int i, j;
     for (i = 0; i < len; i++) {
         crc ^= data[i];
         for (j = 0; j < 8; j++) {
             if (crc & 0x80)
                 crc = (crc << 1) ^ 0x31;
             else
                 crc <<= 1;
         }
     }
     return crc;
 }
 
 static int shtc3_wakeup(struct shtc3_data *data)
 {
     u8 buf[2];
     int ret;
     buf[0] = (SHTC3_CMD_WAKEUP >> 8) & 0xFF;
     buf[1] = SHTC3_CMD_WAKEUP & 0xFF;
     ret = i2c_master_send(data->client, buf, 2);
     if (ret < 0)
         return ret;
     msleep(1); /* Short delay after wakeup */
     return 0;
 }
 
 static int shtc3_sleep(struct shtc3_data *data)
 {
     u8 buf[2];
     int ret;
     buf[0] = (SHTC3_CMD_SLEEP >> 8) & 0xFF;
     buf[1] = SHTC3_CMD_SLEEP & 0xFF;
     ret = i2c_master_send(data->client, buf, 2);
     if (ret < 0)
         return ret;
     return 0;
 }
 
 /*
  * shtc3_measure - Trigger a measurement and read temperature & humidity.
  * Returns temperature in centi-degrees Celsius and humidity in hundredths of %RH.
  */
 static int shtc3_measure(struct shtc3_data *data, int *temperature, int *humidity)
 {
     u8 cmd[2];
     u8 buf[6];
     int ret;
     u16 raw_temp, raw_hum;
     u8 crc;
     mutex_lock(&data->lock);
     ret = shtc3_wakeup(data);
     if (ret < 0) {
         dev_err(&data->client->dev, "Failed to wake up sensor\n");
         goto out_unlock;
     }
     cmd[0] = (SHTC3_CMD_MEASURE >> 8) & 0xFF;
     cmd[1] = SHTC3_CMD_MEASURE & 0xFF;
     ret = i2c_master_send(data->client, cmd, 2);
     if (ret < 0) {
         dev_err(&data->client->dev, "Failed to send measurement command\n");
         goto out_unlock;
     }
     msleep(15);
     ret = i2c_master_recv(data->client, buf, 6);
     if (ret < 0) {
         dev_err(&data->client->dev, "Failed to read sensor data\n");
         goto out_unlock;
     }
     crc = shtc3_crc8(buf, 2);
     if (crc != buf[2]) {
         dev_err(&data->client->dev,
                 "Temperature CRC mismatch: calculated 0x%02x, received 0x%02x\n",
                 crc, buf[2]);
         ret = -EIO;
         goto out_unlock;
     }
     crc = shtc3_crc8(buf + 3, 2);
     if (crc != buf[5]) {
         dev_err(&data->client->dev,
                 "Humidity CRC mismatch: calculated 0x%02x, received 0x%02x\n",
                 crc, buf[5]);
         ret = -EIO;
         goto out_unlock;
     }
     raw_temp = (buf[0] << 8) | buf[1];
     raw_hum  = (buf[3] << 8) | buf[4];
     *temperature = -4500 + (17500 * raw_temp) / 65535;
     *humidity    = (10000 * raw_hum) / 65535;
     ret = 0;
 out_unlock:
     shtc3_sleep(data);
     mutex_unlock(&data->lock);
     return ret;
 }
 
 /* Sysfs 'temperature' attribute */
 static ssize_t temperature_show(struct device *dev,
                                   struct device_attribute *attr, char *buf)
 {
     struct shtc3_data *data = dev_get_drvdata(dev);
     int temperature, humidity, ret;
     ret = shtc3_measure(data, &temperature, &humidity);
     if (ret < 0)
         return ret;
     return scnprintf(buf, PAGE_SIZE, "%d\n", temperature);
 }
 
 /* Sysfs 'humidity' attribute */
 static ssize_t humidity_show(struct device *dev,
                                struct device_attribute *attr, char *buf)
 {
     struct shtc3_data *data = dev_get_drvdata(dev);
     int temperature, humidity, ret;
     ret = shtc3_measure(data, &temperature, &humidity);
     if (ret < 0)
         return ret;
     return scnprintf(buf, PAGE_SIZE, "%d\n", humidity);
 }
 static DEVICE_ATTR_RO(temperature);
 static DEVICE_ATTR_RO(humidity);
 
 static struct attribute *shtc3_attrs[] = {
     &dev_attr_temperature.attr,
     &dev_attr_humidity.attr,
     NULL,
 };
 
 static const struct attribute_group shtc3_attr_group = {
     .attrs = shtc3_attrs,
 };
 
 /*
  * --- Character Device File Operations ---
  */
 static int shtc3_open(struct inode *inode, struct file *file)
 {
     struct shtc3_data *data = container_of(inode->i_cdev, struct shtc3_data, cdev);
     file->private_data = data;
     return 0;
 }
 
 static ssize_t shtc3_read(struct file *file, char __user *buf,
                           size_t count, loff_t *offset)
 {
     struct shtc3_data *data = file->private_data;
     int temperature, humidity, ret;
     char buffer[64];
     int len;
     ret = shtc3_measure(data, &temperature, &humidity);
     if (ret < 0)
         return ret;
     len = scnprintf(buffer, sizeof(buffer), "Temperature: %d, Humidity: %d\n",
                     temperature, humidity);
     if (*offset >= len)
         return 0;
     if (count > len - *offset)
         count = len - *offset;
     if (copy_to_user(buf, buffer + *offset, count))
         return -EFAULT;
     *offset += count;
     return count;
 }
 
 static int shtc3_release(struct inode *inode, struct file *file)
 {
     return 0;
 }
 
 static const struct file_operations shtc3_fops = {
     .owner   = THIS_MODULE,
     .open    = shtc3_open,
     .read    = shtc3_read,
     .release = shtc3_release,
 };
 
 /* Probe function with single parameter */
 static int shtc3_probe(struct i2c_client *client)
 {
     struct shtc3_data *data;
     int ret;
     
     data = devm_kzalloc(&client->dev, sizeof(*data), GFP_KERNEL);
     if (!data)
         return -ENOMEM;
     
     data->client = client;
     mutex_init(&data->lock);
     i2c_set_clientdata(client, data);
     
     ret = sysfs_create_group(&client->dev.kobj, &shtc3_attr_group);
     if (ret) {
         dev_err(&client->dev, "Failed to create sysfs group\n");
         return ret;
     }
     
     ret = alloc_chrdev_region(&data->devt, 0, 1, "shtc3");
     if (ret < 0) {
         dev_err(&client->dev, "Failed to allocate chrdev region\n");
         goto error_sysfs;
     }
     
     cdev_init(&data->cdev, &shtc3_fops);
     data->cdev.owner = THIS_MODULE;
     ret = cdev_add(&data->cdev, data->devt, 1);
     if (ret) {
         dev_err(&client->dev, "Failed to add cdev\n");
         goto error_chrdev;
     }
     
     /* Use the global shtc3_class created in module init */
     data->device = device_create(shtc3_class, NULL, data->devt, data, "%s", client->name);
     if (IS_ERR(data->device)) {
         ret = PTR_ERR(data->device);
         goto error_device;
     }
     
     dev_info(&client->dev, "SHTC3 sensor probed as %s\n", client->name);
     return 0;
     
 error_device:
     cdev_del(&data->cdev);
 error_chrdev:
     unregister_chrdev_region(data->devt, 1);
 error_sysfs:
     sysfs_remove_group(&client->dev.kobj, &shtc3_attr_group);
     return ret;
 }
 
 /* Remove function returning void */
 static void shtc3_remove(struct i2c_client *client)
 {
     struct shtc3_data *data = i2c_get_clientdata(client);
     device_destroy(shtc3_class, data->devt);
     cdev_del(&data->cdev);
     unregister_chrdev_region(data->devt, 1);
     sysfs_remove_group(&client->dev.kobj, &shtc3_attr_group);
 }
 
 /* Manual module init/exit functions instead of module_i2c_driver */
 static int __init shtc3_init(void)
 {
     int ret;
     shtc3_class = class_create("shtc3_class");
     if (IS_ERR(shtc3_class)) {
         pr_err("Failed to create shtc3_class\n");
         return PTR_ERR(shtc3_class);
     }
     ret = i2c_add_driver(&shtc3_driver);
     if (ret)
         class_destroy(shtc3_class);
     return ret;
 }
 
 static void __exit shtc3_exit(void)
 {
     i2c_del_driver(&shtc3_driver);
     class_destroy(shtc3_class);
 }
 
 module_init(shtc3_init);
 module_exit(shtc3_exit);
 
 /* i2c_driver structure */
 static struct i2c_driver shtc3_driver = {
     .driver = {
         .name = "shtc3",
         .of_match_table = of_match_ptr(shtc3_of_match),
     },
     .probe    = shtc3_probe,
     .remove   = shtc3_remove,
     .id_table = shtc3_id,
 };
 
 MODULE_AUTHOR("Sanjay");
 MODULE_DESCRIPTION("SHTC3 Temperature and Humidity Sensor Driver with Char Device");
 MODULE_LICENSE("GPL");
 