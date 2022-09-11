// Arduino value clamping.
// Copyright 2021 Jason Pepas.
// Released under the terms of the MIT License, see https://opensource.org/licenses/MIT

#ifndef _CLAMP_H_
#define _CLAMP_H_

float clampf(float value, float min, float max);

#endif

#ifdef _CLAMP_H_IMPLEMENTATION_

float clampf(float value, float min, float max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}

#endif
