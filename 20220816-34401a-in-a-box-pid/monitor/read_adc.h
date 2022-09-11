// Arduino ADC oversampling.
// Copyright 2021 Jason Pepas.
// Released under the terms of the MIT License, see https://opensource.org/licenses/MIT

#ifndef _READ_ADC_H
#define _READ_ADC_H

#include "types.h"

typedef uint8_t oversample_t;  // The degree of oversampling (e.g. 16x).
#define OVERSAMPLE_1x 1
#define OVERSAMPLE_2x 2
#define OVERSAMPLE_4x 4
#define OVERSAMPLE_8x 8
#define OVERSAMPLE_16x 16
#define OVERSAMPLE_32x 32
#define OVERSAMPLE_64x 64

uint8_t shift_value_for(oversample_t oversample);
adc_t read_adc(pin_t sensor_pin, oversample_t oversample);

#endif

#ifdef _READ_ADC_H_IMPLEMENTATION_

// Returns the shift value corresponding to the oversampling rate.
// Usage: accumulator >>= shift_value_for(OVERSAMPLE_16x);
uint8_t shift_value_for(oversample_t oversample) {
    switch (oversample) {
        case 1: return 0;
        case 2: return 1;
        case 4: return 2;
        case 8: return 3;
        case 16: return 4;
        case 32: return 5;
        case 64: return 6;
        default: return 255;
    }
}

// ADC read with oversampling.
// Usage: adc_t value = read_adc(my_sensor_pin, OVERSAMPLE_16x);
adc_t read_adc(pin_t sensor_pin, oversample_t oversample) {
    // Note: arduino analogRead takes about 100us (10kHz) according to
    // http://yaab-arduino.blogspot.com/2015/02/fast-sampling-from-analog-input.html
    // An oversample rate of 64x fits 10-bit values neatly into a 16-bit unsigned accumulator, at about 150Hz.
    uint16_t accumulator = 0;
    for (uint8_t i=0; i < oversample; i++) {
        int reading = analogRead(sensor_pin);
        accumulator += reading;
    }
    accumulator >>= shift_value_for(oversample);
    return (adc_t)accumulator;
}

// ADC read with oversampling, returning a float.
// Usage: float value = read_adc(my_sensor_pin, OVERSAMPLE_16x);
float read_adc_f(pin_t sensor_pin, oversample_t oversample) {
    // Note: arduino analogRead takes about 100us (10kHz) according to
    // http://yaab-arduino.blogspot.com/2015/02/fast-sampling-from-analog-input.html
    // An oversample rate of 64x fits 10-bit values neatly into a 16-bit unsigned accumulator, at about 150Hz.
    uint16_t accumulator = 0;
    for (uint8_t i=0; i < oversample; i++) {
        int reading = analogRead(sensor_pin);
        accumulator += reading;
    }
    return accumulator / (float)oversample;
}

#endif
