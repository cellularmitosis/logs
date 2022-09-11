#ifndef _ROLLAVGF_H_
#define _ROLLAVGF_H_

#include <stdint.h>

struct _rollavgf_t {
    uint8_t size;
    float* buf;
};
typedef struct _rollavgf_t rollavgf_t;

void rollavgf_init(rollavgf_t* ravg, uint8_t size, float* buf, float initial_value);
float rollavgf_push(rollavgf_t* ravg, float value);
float rollavgf_get(rollavgf_t* ravg);

#endif

#ifdef _ROLLAVGF_H_IMPLEMENTATION_

void rollavgf_init(rollavgf_t* ravg, uint8_t size, float* buf, float initial_value) {
    ravg->size = size;
    ravg->buf = buf;
    for (int i=0; i < ravg->size; i++) {
        ravg->buf[i] = initial_value;
    }
}

float rollavgf_push(rollavgf_t* ravg, float value) {
    float accum = 0;
    for (int i = ravg->size - 1; i > 0; i--) {
        ravg->buf[i] = ravg->buf[i-1];
        accum += ravg->buf[i];
    }
    ravg->buf[0] = value;
    return rollavgf_get(ravg);
}

float rollavgf_get(rollavgf_t* ravg) {
    float accum = 0;
    for (int i=0; i < ravg->size; i++) {
        accum += ravg->buf[i];
    }
    return accum / ravg->size;
}

#endif
