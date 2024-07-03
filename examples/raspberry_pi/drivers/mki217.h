#ifndef DRIVERS_MKI217_H
#define DRIVERS_MKI217_H

#include <cstdint>

/**
 * Initialize the MKI217 sensors
 */
bool mki217_init(uint8_t i2cAddr, const char *i2cBus);

/**
 * Read the temperature from the MKI217 sensor
 * @param temp the temperature in degrees Celsius
 * @return true if the temperature was read successfully, false otherwise
 */
bool mki217_temp(float &temp);

/**
 * Read the accelerometer from the MKI217 sensor
 * @param x the x-axis acceleration
 * @param y the y-axis acceleration
 * @param z the z-axis acceleration
 * @return true if the accelerometer was read successfully, false otherwise
 */
bool mki217_accel(float &x, float &y, float &z);

#endif //DRIVERS_MKI217_H