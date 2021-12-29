#ifndef SYSTEM_H
#define SYSTEM_H

#include <esp32fota.h>

#include <esp_system.h>
#include <esp_sleep.h>

#include "sensor.h"
#include "eeprom.h"
#include "timing.h"
#include "flag.h"

#define URL_OTA ""

#define EXT1_WAKEUP_MASK_PIN 0x1000000000

static void sys_init(){
    gpio_set_direction(GPIO_NUM_32, GPIO_MODE_INPUT);

    esp_sleep_enable_ext0_wakeup(GPIO_NUM_35, 0);
}

esp32FOTA esp32FOTA(BX_DEVICE_TYPE,FW_VER,false);

bx_err_check_ota check_n_update_fw(){

    esp32FOTA.checkURL = URL_OTA;

    Serial.println("[system] checking update...");

    bool updateAvailable = esp32FOTA.execHTTPcheck();

    if(updateAvailable){

        Serial.println("[system] new version of firmware detected, begin update...");
        esp32FOTA.execOTA();

    }

    vTaskDelay(2000/portTICK_RATE_MS);

}

static void sys_generate_hw_id(char id[]){
    
    uint32_t hw_id[3];
    uint32_t tmp;
    char str_hw_id[16];

    Serial.printf("[system] generating hardware id\r\n");

    for(uint8_t x=0; x<3; x++){

        tmp = esp_random()/1000000;
        tmp = ((tmp < 1000) ? ((1000 - tmp) + (esp_random()/1000000)) + tmp : tmp);
        hw_id[x] = tmp;
    }

    snprintf(
        str_hw_id,
        sizeof(str_hw_id),
        "%d-%d-%d",
        hw_id[0],
        hw_id[1],
        hw_id[2]
    );

    Serial.printf("[system] generated id : %s\r\n", str_hw_id);

    for(uint8_t x=0; x<sizeof(str_hw_id); x++)
        id[x] = str_hw_id[x];
}

static void system_sleep(){
    flag_deinit();
    Serial.printf("[system] sleep..\r\n");

    esp_deep_sleep_start();
}

static void system_throw_json_format(char str[], char hwid[14]){
    char data[255];
    char str_time[100];
    String err_msg;

    // dd-mmm-yyyy hh:mm:ss
    snprintf(
        str_time,
        sizeof(str_time),
        "%d-%d-%d-%d-%d-%d",
        timing_t.year,
        timing_t.month,
        timing_t.day,
        timing_t.hour,
        timing_t.minute,
        timing_t.second
    );

    err_msg = flag_throw_desc_msg();

#ifdef BX_DEVICE_AMBIENT
    snprintf(
        data,
        sizeof(data),
        "{temperature:%f,humidity:%f,id:\"%s\",CO2:%f,voc:%d,time:\"%s\",timestamp:%llu,status:\"%s\"}",
        sensor_data_t.temperature,
        sensor_data_t.humidity,
        hwid,
        sensor_data_t.CO2,
        sensor_data_t.raw_voc,
        str_time,
        timing_epoch_ms_now(),
        err_msg.c_str()
    );
#elif BX_DEVICE_SCALE
#elif BX_DEVICE_ANEMO
#endif

    strncpy(str, data, sizeof(data));
}

static void system_print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  String msg = "";

  Serial.print("[system] ");
  switch(wakeup_reason)
  {
    case 1  : msg = "Wakeup caused by external signal using RTC_IO"; break;
    case 2  : msg = "Wakeup caused by external signal using RTC_CNTL"; break;
    case 3  : msg = "Wakeup caused by timer"; break;
    case 4  : msg = "Wakeup caused by touchpad"; break;
    case 5  : msg = "Wakeup caused by ULP program"; break;
    default : msg = "Wakeup was not caused by deep sleep"; break;
  }

  Serial.println(msg);
  
}

#endif