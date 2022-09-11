// This PID implementation is modified from "Phil's Lab", which is MIT licensed.
// See https://github.com/pms67/PID
// See https://www.youtube.com/watch?v=zOByx3Izf5U
// This modified version is also MIT licensed.
// See see https://opensource.org/licenses/MIT

#ifndef _PID_H_
#define _PID_H_

#include "clamp.h"

struct _pid_t {
    // Controller gains.
    float Kp;
    float Ki;
    float Kd;

    // Derivative low-pass filter time constant.
    // From the youtube video:
    // Additional note: The derivative low-pass filter can be controlled by the
    // constant 'tau', which is the time constant of the filter (-3dB frequency
    // in Hz, fc = 1 / (2*pi*tau)). A larger value of tau means the signal is
    // filtered more heavily. As tau approaches zero, the differentiator
    // approaches a 'pure differentiator' with no filtering.
    float tau;

    // Output limits.
    // Integrator does not wind up when output is clamped.
    float output_min;
    float output_max;

    // Controller "memory".
    float integrator;
    float prev_error;  // Required for integrator.
    float differentiator;
    float prev_measurement;  // Required for differentiator.
};
typedef struct _pid_t pid_t;

typedef float seconds_t;

void pid_init(pid_t *pid);
float pid_update(pid_t *pid, float setpoint, float measurement, seconds_t period);

#endif

#ifdef _PID_H_IMPLEMENTATION_

void pid_init(pid_t *pid) {
    pid->integrator = 0.0f;
    pid->prev_error  = 0.0f;
    pid->differentiator  = 0.0f;
    pid->prev_measurement = 0.0f;
}

float pid_update(pid_t *pid, float setpoint, float measurement, seconds_t period) {
    float error = setpoint - measurement;

    float proportional = pid->Kp * error;

    float previous_integrator = pid->integrator;
    pid->integrator = pid->integrator + 0.5f * pid->Ki * period * (error + pid->prev_error);

    // Derivative (band-limited differentiator)
    // Note: derivative on measurement, therefore minus sign in front of equation!
    pid->differentiator = -(2.0f * pid->Kd * (measurement - pid->prev_measurement)
                        + (2.0f * pid->tau - period) * pid->differentiator)
                        / (2.0f * pid->tau + period);

    // Store error and measurement for later use.
    pid->prev_error = error;
    pid->prev_measurement = measurement;

    float output = proportional + pid->integrator + pid->differentiator;
    if (output < pid->output_min || output > pid->output_max) {
        // Do not wind-up the integrator while the output is clamped.
        pid->integrator = previous_integrator;
    }
    output = clampf(output, pid->output_min, pid->output_max);
    return output;
}

#endif
