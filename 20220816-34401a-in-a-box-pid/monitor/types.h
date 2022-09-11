// The Arduino preprocessor rearranges code, which sometimes moves function
// declarations above typedefs.  If those functions rely on those types,
// this will cause a compilation error.
// The work-around is to move typedefs out into a header file, to ensure
// they are defined before the relocated function declarations.

#include <stdint.h>

typedef int pin_t;  // an Arduino pin number.
typedef int adc_t;  // An Arduino 10-bit ADC value (0-1023).
typedef int pwm_t;  // an Arduino PWM output value (0-255).
typedef uint32_t millis_t;  // Milliseconds as returned by millis().

typedef float amps_t;  // Amps.
typedef float volts_t;  // Volts.
typedef float watts_t;  // Watts.
