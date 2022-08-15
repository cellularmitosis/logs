// arduino sketch which uses two HTU21D sensors to monitor the internal and
// external temperature of a cardbboard box containing a running 34401A.

#include <Wire.h>
#include "HTU2xD_SHT2x_Si70xx.h"

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

void setup() {
    Serial.begin(9600);
    Serial.print(F("\n"));

    reset_and_wait_for_htu21d(&int_htu21d, F("Error: int_htu21d.begin().\n"));
    reset_and_wait_for_htu21d(&ext_htu21d, F("Error: ext_htu21d.begin().\n"));

    float id;
    id = int_htu21d.readDeviceID();
    while (true) {
        if (id == HTU2XD_SHT2X_SI70XX_ERROR) {
            reset_and_wait_for_htu21d(&int_htu21d, F("Error: int_htu21d.readDeviceID().\n"));
            continue;
        } else {
            Serial.print(F("Found hardware I2C device with id: "));
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

void loop() {
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
