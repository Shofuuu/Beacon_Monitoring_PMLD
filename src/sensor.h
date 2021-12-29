#ifndef SENSOR_H
#define SENSOR_H

#include "bx_err.h"
#include <Wire.h>
#include <WSEN_HIDS_CPP.h>
#include <Adafruit_SGP40.h>
#include <SparkFun_SCD4x_Arduino_Library.h>

Sensor_HIDS wsen;
Adafruit_SGP40 sgp;
SCD4x scd;

uint8_t sampling_data = 50;

struct{
    float data_temperature_tp = 0;
    float data_humidity_tp = 0;
}wsen_param_t;

struct{
    float data_voc_tp;
}sgp_param_t;

struct{
    float data_co2_tmp = 0;
}scd_param_t;

struct{

    float temperature;
    float humidity;
    float CO2;
    float amonia;
    uint16_t raw_voc;

}sensor_data_t;

bool measureCO2 = true, measureVOC = true;

bx_err_sensor sensor_init(){

    Wire.begin();

    if(scd.begin()==false){
        Serial.println("[sensor] CO2 sensor not detected!");
        measureCO2 = false;
    }
    else{
        if(scd.stopPeriodicMeasurement()==true)
            Serial.println(F("[sensor] CO2 sensor periodic measurement is disabled!"));
        if(scd.startLowPowerPeriodicMeasurement()==true)
            Serial.println(F("[sensor] CO2 sensor low power mode enabled!"));
    }

    wsen.init(HIDS_ADDRESS_I2C_0);
    wsen.set_continuous_mode(1);
    Serial.println("[sensor] RHT sensor init");
    
    if(!sgp.begin()){
        Serial.println("[sensor] voc sensor failed!");
        measureVOC = false;
    }
    else
        Serial.println("[sensor] voc sensor init");

    vTaskDelay(5000/portTICK_PERIOD_MS);

    return BX_SENSOR_OK;

}

bx_err_sensor sensor_throw_sensor_value(void){

    uint8_t counter_t   = 0;
    uint8_t counter_h   = 0;
    uint8_t counter_voc = 0;
    uint8_t counter_co2 = 0;

    Serial.println("[Sensor] Measuring temperature...");

    while(counter_t <= sampling_data){
        wsen_param_t.data_temperature_tp += wsen.get_Temperature();
        counter_t++;
        vTaskDelay(100/portTICK_PERIOD_MS);
    }

    sensor_data_t.temperature = wsen_param_t.data_temperature_tp / (counter_t + 1);

    Serial.println("[Sensor] measuring humidity...");

    while(counter_h <= sampling_data){
        wsen_param_t.data_humidity_tp += wsen.get_Humidity();
        counter_h++;
        vTaskDelay(100/portTICK_PERIOD_MS);
    }

    sensor_data_t.humidity = wsen_param_t.data_humidity_tp / (counter_h + 1);

    if(sensor_data_t.humidity >= 100)
        sensor_data_t.humidity = 100;

    if(measureVOC){

        Serial.println("[sensor] measuring voc...");

        while(counter_voc <= sampling_data){
            sgp_param_t.data_voc_tp += sgp.measureRaw(25.0,50.0);
            counter_voc++;
            vTaskDelay(100/portTICK_PERIOD_MS);
        }

        sensor_data_t.raw_voc = sgp_param_t.data_voc_tp / (counter_voc + 1);
    }
    else{
        Serial.println("[sensor] voc sensor not installed. skipping measurement...");
        sensor_data_t.raw_voc = 0;
    }

    if(measureCO2){

        Serial.println("[sensor] measuring CO2...");

        while(counter_co2 <= sampling_data){
            scd_param_t.data_co2_tmp += scd.getCO2();
            counter_co2++;
            vTaskDelay(100/portTICK_PERIOD_MS);
        }

        sensor_data_t.CO2 = scd_param_t.data_co2_tmp / (counter_co2 + 1);
    }
    else{
        Serial.println("[sensor] CO2 sensor not installed. skipping measurement...");
        sensor_data_t.CO2 = 0;
    }

    Serial.println("[sensor] measurement complete");

    return BX_SENSOR_OK;
}

#endif