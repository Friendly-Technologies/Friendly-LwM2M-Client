#ifndef DRIVER_GPIO_H
#define DRIVER_GPIO_H

/**
 * Set the state of a pin
 * @param pin the pin number
 * @param state the state to set the pin to
 * @return true if the pin state was set successfully, false otherwise
 */
bool setPinState(int pin, bool state);

/**
 * Get the state of a pin
 * @return true if the pin is high, false if the pin is low
 */
bool getPinState(int pin);

#endif //DRIVER_GPIO_H