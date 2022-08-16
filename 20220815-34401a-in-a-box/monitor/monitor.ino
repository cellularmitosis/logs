// arduino sketch which uses two HTU21D sensors to monitor the internal and
// external temperature of a cardbboard box containing a running 34401A.

#include "types.h"

#include <Wire.h>
#include "HTU2xD_SHT2x_Si70xx.h"

#define _READ_ADC_H_IMPLEMENTATION_
#include "read_adc.h"

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
    float vref = 1.1f;
    float r1 = 47000.0f;
    float r2 = 10000.0f;
    float fudge = 0.979f;
    float vcc = pin_v / 1024.0f * vref * (r1 + r2) / r2 * fudge;
    return vcc;
}

void setup() {
    Serial.begin(9600);
    Serial.print(F("\n"));

    pinMode(pwm_pin, OUTPUT);

    analogReference(INTERNAL);
    // Throw away some samples after switch AREF:
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

    Serial.print("CSV output:\n");
    delay(250);
    Serial.print("time_s,int_temp_c,int_temp_f,int_humid,ext_temp_c,ext_temp_f,ext_humid\n");
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

void loop() {
    // calibrate(); return;

    while (millis() % 1000 > 3) {
        continue;
    }

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

    analogWrite(pwm_pin, 100);

    Serial.print(millis() / 1000);
    Serial.print(F(","));
    Serial.print(int_temp_c);
    Serial.print(F(","));
    Serial.print(c_to_f(int_temp_c));
    Serial.print(F(","));
    Serial.print(int_humidity);
    Serial.print(F(","));
    Serial.print(ext_temp_c);
    Serial.print(F(","));
    Serial.print(c_to_f(ext_temp_c));
    Serial.print(F(","));
    Serial.print(ext_humidity);
    Serial.print(F("\n"));
}
