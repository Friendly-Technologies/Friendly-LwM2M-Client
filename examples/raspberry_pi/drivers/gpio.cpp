#include "gpio.h"
#include <gpiod/gpiod.h>
#include <iostream>

using namespace std;

bool setPinState(int pin, bool state) {
    struct gpiod_line *line;
    struct gpiod_chip *chip;

    const char *chipName= "gpiochip0";
    const char *consumer= "gpio-toggle";
    
    chip = gpiod_chip_open_by_name(chipName);
    if (!chip) {
        cout << "GPIO: error opening chip" << endl;
        return false;
    }
    
    line =  gpiod_chip_get_line(chip, pin);
    if (!line) {
        cout << "GPIO: error opening line"<< endl;
        gpiod_chip_close(chip);
        return false;
    }
    gpiod_line_request_output(line, consumer, 0);
    gpiod_line_set_value(line, state);

    gpiod_line_release(line);
    gpiod_chip_close(chip);

    return true;
}

bool getPinState(int pin) {
    struct gpiod_line *line;
    struct gpiod_chip *chip;

    const char *chipName= "gpiochip0";
    const char *consumer= "gpio-toggle";
    
    chip = gpiod_chip_open_by_name(chipName);
    if (!chip) {
        cout << "GPIO: error opening chip" << endl;
        return false;
    }
    
    line =  gpiod_chip_get_line(chip, pin);
    if (!line) {
        cout << "GPIO: error opening line"<< endl;
        gpiod_chip_close(chip);
        return false;
    }

    gpiod_line_request_input(line, consumer);
    int value = gpiod_line_get_value(line);

    gpiod_line_release(line);
    gpiod_chip_close(chip);

    return value;
}