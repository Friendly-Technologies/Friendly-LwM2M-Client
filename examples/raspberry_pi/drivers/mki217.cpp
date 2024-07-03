#include "mki217.h"

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <cmath>

#define CTRL1_XL 0x10
#define CTRL2_G 0x11
#define CTRL3_C 0x12
#define OUT_TEMP_L 0x20
#define OUT_TEMP_H 0x21
#define OUTX_L_G 0x22
#define OUTX_H_G 0x23
#define OUTY_L_G 0x24
#define OUTY_H_G 0x25
#define OUTZ_L_G 0x26
#define OUTZ_H_G 0x27
#define OUTX_L_A 0x28
#define OUTX_H_A 0x29
#define OUTY_L_A 0x2A
#define OUTY_H_A 0x2B
#define OUTZ_L_A 0x2C
#define OUTZ_H_A 0x2D

// Sensitivity factors (example values from datasheet)
#define ACCEL_SENSITIVITY (0.061f)
#define GYRO_SENSITIVITY (8.75f)

// Temperature sensor offset (assuming datasheet specifies this)
#define TEMPERATURE_OFFSET (25.0)

// Conversion factor for accelerometer mg to m/s²
#define MG_TO_MS2 (9.80665 / 1000)

static int i2c_fd = -1;

static int16_t read_axis(int low_reg, int high_reg) {
    char reg[1] = {0};
    char data[1] = {0};
    
    reg[0] = low_reg;
    write(i2c_fd, reg, 1);
    read(i2c_fd, data, 1);
    int low_byte = data[0];
    
    reg[0] = high_reg;
    write(i2c_fd, reg, 1);
    read(i2c_fd, data, 1);
    int high_byte = data[0];
    
    int16_t axis = (high_byte << 8) | low_byte;
    if (axis & (1 << 15)) {
        axis -= (1 << 16);
    }
    return axis;
}

bool mki217_init(uint8_t i2cAddr, const char *i2cBus) {
    char config[2] = {0};

    if (i2c_fd != -1) {
        std::cerr << "MKI217 sensor already initialized" << std::endl;
        return false;
    }
    
    // Open I2C bus
    if ((i2c_fd = open(i2cBus, O_RDWR)) < 0) {
        std::cerr << "Failed to open the I2C bus" << std::endl;
        return false;
    }
    
    // Set the I2C address
    if (ioctl(i2c_fd, I2C_SLAVE, i2cAddr) < 0) {
        std::cerr << "Failed to acquire bus access and/or talk to slave" << std::endl;
        close(i2c_fd);
        return false;
    }

    // Reset the sensor
    config[0] = CTRL3_C;
    config[1] = 0x01;
    write(i2c_fd, config, 2);
    usleep(100000);

    // Enable Block Data Update (BDU) and Auto-increment
    config[0] = CTRL3_C;
    config[1] = 0x44;
    write(i2c_fd, config, 2);

    // Enable the accelerometer and gyroscope
    config[0] = CTRL1_XL;
    config[1] = 0x60;  // 208 Hz, 2g, 100 Hz filter
    write(i2c_fd, config, 2);
    config[0] = CTRL2_G;
    config[1] = 0x60;  // 208 Hz, 250 dps
    write(i2c_fd, config, 2);

    return true;
}

/**
 * Read the temperature from the MKI217 sensor
 * @param temp the temperature in degrees Celsius
 * @return true if the temperature was read successfully, false otherwise
 */
bool mki217_temp(float &temp) {
    temp = read_axis(OUT_TEMP_L, OUT_TEMP_H);
    temp = (temp / 256.0) + TEMPERATURE_OFFSET;
    return true;
}

/**
 * Read the accelerometer from the MKI217 sensor
 * @param x the x-axis acceleration
 * @param y the y-axis acceleration
 * @param z the z-axis acceleration
 * @return true if the accelerometer was read successfully, false otherwise
 */
bool mki217_accel(float &x, float &y, float &z) {
    x = read_axis(OUTX_L_A, OUTX_H_A) * ACCEL_SENSITIVITY * MG_TO_MS2;
    y = read_axis(OUTY_L_A, OUTY_H_A) * ACCEL_SENSITIVITY * MG_TO_MS2;
    z = read_axis(OUTZ_L_A, OUTZ_H_A) * ACCEL_SENSITIVITY * MG_TO_MS2;
    return true;
}