#ifndef SENSOR_H
#define SENSOR_H

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#include "bme280.h"

struct identifier {
    uint8_t dev_addr;
    int8_t fd;
};


class Sensor {
    private:
    int8_t rslt;

    public:
    Sensor(char *name, struct bme280_dev *dev, struct identifier *id);
    double getSensorTemp(struct bme280_dev *dev);
    static int8_t i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);
    static int8_t i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);
    static void user_delay_us(uint32_t period, void *intf_ptr);

};

#endif