// arduino sketch which uses two HTU21D sensors to monitor the internal and
// external temperature of a cardboard box containing a running 34401A, and
// control a heater via PID to regulate the temperature of the cardboard box.

#include <Wire.h>
#include "HTU2xD_SHT2x_Si70xx.h"

#include "types.h"

#define _READ_ADC_H_IMPLEMENTATION_
#include "read_adc.h"

#define _PID_H_IMPLEMENTATION_
#include "pid.h"

#define _CLAMP_H_IMPLEMENTATION_
#include "clamp.h"

#define _ROLLAVGF_H_IMPLEMENTATION_
#include "rollavgf.h"

// the pwm output pin which controls the op amp current regulation loop.
uint8_t pwm_pin = 9;

// the internal temperature sensor
uint8_t int_scl_pin = 12;
uint8_t int_sda_pin = 11;
SoftwareWire int_swire(int_sda_pin, int_scl_pin);
HTU2xD_SHT2x_SI70xx int_htu21d(HTU2xD_SENSOR, HUMD_12BIT_TEMP_14BIT, &int_swire);

// the external temperature sensor
uint8_t ext_scl_pin = 7;
uint8_t ext_sda_pin = 6;
SoftwareWire ext_swire(ext_sda_pin, ext_scl_pin);
HTU2xD_SHT2x_SI70xx ext_htu21d(HTU2xD_SENSOR, HUMD_12BIT_TEMP_14BIT, &ext_swire);

// PID config and state:
pid_t g_pid;

float g_target_temp_c = 32.0;

float c_to_f(float c) {
    return (c * 9.0f/5.0f) + 32.0f;
}

void reset_and_wait_for_htu21d(HTU2xD_SHT2x_SI70xx* sensor, __FlashStringHelper* message) {
    while (sensor->begin() != true) {
        Serial.print(message);
        delay(1000);
    }
}

// Read the 1.1v internal Vref via a 47k / 10k divider on pin A0.
float read_vcc() {
    uint8_t vcc_sense_pin = A0;
    float pin_v = read_adc_f(vcc_sense_pin, OVERSAMPLE_64x);
    float vref = 1.1;
    float r1 = 47000.0;
    float r2 = 10000.0;
    float fudge = 0.979;
    float vcc = pin_v / 1024.0 * vref * (r1 + r2) / r2 * fudge;
    return vcc;
}

pwm_t amps_to_pwm(amps_t amps) {
    // PWM vs Volts (at load), Amps:
    // 0: 12.004V
    // 25: 11.999V 0.266A
    // 50: 11.983V, 0.511A 
    // 75: 11.970V, 0.756A
    if (amps <= 0.266) {
        return amps / 0.266 * 25.0;
    } else if (amps <= 0.511) {
        return amps / 0.511 * 50.0;
    } else if (amps <= 0.756) {
        return amps / 0.756 * 75.0;
    // 100: 11.959V, 1.001A
    // 125: 11.948V, 1.246A
    // 150: 11.935V, 1.490A
    // 175: 11.921V, 1.736A
    } else if (amps <= 1.001) {
        return amps / 1.001 * 100.0;
    } else if (amps <= 1.246) {
        return amps / 1.246 * 125.0;
    } else if (amps <= 1.490) {
        return amps / 1.490 * 150.0;
    } else if (amps <= 1.736) {
        return amps / 1.736 * 175.0;
    // 200: 11.907V, 1.981A
    // 225: 11.893V, 2.226A
    // 250: 11.877V, 2.472A
    } else if (amps <= 1.981) {
        return amps / 1.981 * 200.0;
    } else if (amps <= 2.226) {
        return amps / 2.226 * 225.0;
    } else if (amps <= 2.472) {
        return amps / 2.472 * 250.0;
    } else {
        return 0;
    }
}

void calibrate() {
    // PWM vs voltage at end of filter (before divider):
    // 25: 0.485v
    // 50: 0.969v
    // 75: 1.936v
    // 100: 3.862v
    
    // PWM vs Volts (at load), Amps:
    // 0: 12.004V, 0.266A
    // 25: 11.999V 0.266A
    // 50: 11.983V, 0.511A 
    // 75: 11.970V, 0.756A
    // 100: 11.959V, 1.000A
    // 125: 11.948V, 1.246A
    // 150: 11.935V, 1.490A
    // 175: 11.921V, 1.736A
    // 200: 11.907V, 1.981A
    // 225: 11.893V, 2.226A
    // 250: 11.877V, 2.472A

    Serial.println("pwm 0");
    analogWrite(pwm_pin, 0);
    delay(10000);
    Serial.println("pwm 25");
    analogWrite(pwm_pin, 25);
    delay(10000);
    Serial.println("pwm 50");
    analogWrite(pwm_pin, 50);
    delay(10000);
    Serial.println("pwm 75");
    analogWrite(pwm_pin, 75);
    delay(10000);
    Serial.println("pwm 100");
    analogWrite(pwm_pin, 100);
    delay(10000);
    Serial.println("pwm 125");
    analogWrite(pwm_pin, 125);
    delay(10000);
    Serial.println("pwm 150");
    analogWrite(pwm_pin, 150);
    delay(10000);
    Serial.println("pwm 175");
    analogWrite(pwm_pin, 175);
    delay(10000);
    Serial.println("pwm 200");
    analogWrite(pwm_pin, 200);
    delay(10000);
    Serial.println("pwm 225");
    analogWrite(pwm_pin, 225);
    delay(10000);
    Serial.println("pwm 250");
    analogWrite(pwm_pin, 250);
    delay(10000);
}

rollavgf_t g_ravg;
float ravg_buf[8];

void setup() {
    Serial.begin(9600);
    Serial.print(F("\n"));

    // See also https://www.ctrlaltftc.com/tuning-methods-of-a-pid-controller
    g_pid.Kp = 16.0;
    g_pid.Ki = 0.0;
    g_pid.Kd = 8.0;
    g_pid.tau = 0.1;
    g_pid.output_max = 1.95;
    g_pid.output_min = 0.01;

    pinMode(pwm_pin, OUTPUT);

    analogReference(INTERNAL);
    // Throw away some samples after switching AREF:
    read_adc_f(A0, OVERSAMPLE_64x);

    reset_and_wait_for_htu21d(&int_htu21d, F("Error: int_htu21d.begin().\n"));
    reset_and_wait_for_htu21d(&ext_htu21d, F("Error: ext_htu21d.begin().\n"));

    float id;
    id = int_htu21d.readDeviceID();
    while (true) {
        if (id == HTU2XD_SHT2X_SI70XX_ERROR) {
            reset_and_wait_for_htu21d(&int_htu21d, F("Error: int_htu21d.readDeviceID().\n"));
            continue;
        } else {
            Serial.print(F("Found software I2C device with id: "));
            Serial.print((int)id);
            Serial.print(F("\n"));
            break;
        }
    }

    id = ext_htu21d.readDeviceID();
    while (true) {
        if (id == HTU2XD_SHT2X_SI70XX_ERROR) {
            reset_and_wait_for_htu21d(&ext_htu21d, F("Error: ext_htu21d.readDeviceID().\n"));
            continue;
        } else {
            Serial.print(F("Found software I2C device with id: "));
            Serial.print((int)id);
            Serial.print(F("\n"));
            break;
        }
    }

    rollavgf_init(&g_ravg, 8, ravg_buf, 25.0);

    Serial.print("CSV output:\n");
    delay(250);
    Serial.print("time_s,int_temp_c,int_temp_f,int_humid,ext_temp_c,ext_temp_f,ext_humid,ravg_int_temp_c,ext_temp_c+4,amps,pwm,pid_i,pid_d,target_c\n");
}

void loop() {
    // calibrate(); return;

    while ((millis() % 1000) >= 3) {
        continue;
    }
    uint32_t elapsed = millis() / 1000;

    float int_humidity = int_htu21d.readHumidity();
    float int_temp_c = int_htu21d.readTemperature(READ_TEMP_AFTER_RH);
    if (int_humidity == HTU2XD_SHT2X_SI70XX_ERROR || int_temp_c == HTU2XD_SHT2X_SI70XX_ERROR) {
        reset_and_wait_for_htu21d(&int_htu21d, F("Error: int_htu21d.readHumidity() or int_htu21d.readTemperature()\n"));
        int_htu21d.begin();
        return;
    }

    float ext_humidity = ext_htu21d.readHumidity();
    float ext_temp_c = ext_htu21d.readTemperature(READ_TEMP_AFTER_RH);
    if (ext_humidity == HTU2XD_SHT2X_SI70XX_ERROR || ext_temp_c == HTU2XD_SHT2X_SI70XX_ERROR) {
        reset_and_wait_for_htu21d(&ext_htu21d, F("Error: ext_htu21d.readHumidity() or ext_htu21d.readTemperature()\n"));
        ext_htu21d.begin();
        return;
    }

    float ravg_int_temp_c = rollavgf_push(&g_ravg, int_temp_c);

    watts_t calculated_watts = pid_update(&g_pid, g_target_temp_c, ravg_int_temp_c, 1);
    volts_t power_supply_volts = 12.0;
    amps_t total_amps = calculated_watts / power_supply_volts;
    float number_of_loads = 2.0;
    amps_t per_channel_amps = total_amps / number_of_loads;
    pwm_t pwm = amps_to_pwm(per_channel_amps);
    analogWrite(pwm_pin, pwm);

    Serial.print(millis() / 1000);
    Serial.print(F(","));
    Serial.print(int_temp_c, 4);
    Serial.print(F(","));
    Serial.print(c_to_f(int_temp_c), 5);
    Serial.print(F(","));
    Serial.print(int_humidity, 4);
    Serial.print(F(","));
    Serial.print(ext_temp_c, 4);
    Serial.print(F(","));
    Serial.print(c_to_f(ext_temp_c), 5);
    Serial.print(F(","));
    Serial.print(ext_humidity, 4);
    Serial.print(F(","));
    Serial.print(ravg_int_temp_c, 6);
    Serial.print(F(","));
    Serial.print(ext_temp_c + 4.0, 4); // external temp + 4C rise from meter self-heating.
    Serial.print(F(","));
    Serial.print(calculated_amps, 3);
    Serial.print(F(","));
    Serial.print(pwm);
    Serial.print(F(","));
    Serial.print(g_pid.integrator, 4);
    Serial.print(F(","));
    Serial.print(g_pid.differentiator, 4);
    Serial.print(F(","));
    Serial.print(g_target_temp_c, 4);
    Serial.print(F("\n"));
    delay(10);
}
