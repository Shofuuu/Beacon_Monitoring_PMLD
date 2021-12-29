#include <Arduino.h>

#include <esp_log.h>
#include <esp_err.h>

#include <freertos/FreeRTOS.h>

#include "system.h"
#include "bx_err.h"
#include "flag.h"
#include "sensor.h"
#include "timing.h"
#include "mqtt.h"
#include "wifi.h"

uint8_t interval;

void setup(){

  Serial.begin(115200);
  while(!Serial);

  system_print_wakeup_reason();
  sys_init();

  eeprom_init();
  flag_init();
  ble_init();
  sensor_init();

  char hw_id[16];
  uint8_t ssid_len = 0;
  uint8_t pwd_len = 0;
  uint8_t access_token_len = 0;
  uint8_t mqtt_url_len = 0;

  bx_err_eeprom err;
  bx_err_timing err_timing;

#ifdef BX_CALIBRATE_RTC
  err_timing = timing_init(1);

  if(err_timing == BX_TIMING_ERR)
    Serial.println("[rtc] error calibrating! skip.");
  else
    Serial.println("[rtc] calibrated successfully");
#endif

#ifdef BX_NEW_DEVICE

  err_timing = timing_init(1);

  if(err_timing == BX_TIMING_ERR)
    Serial.println("[rtc] error occured! skip.");
  else
    Serial.println("[rtc] reset the time");

  char ssid[64] = "monitoring-kandang";
  char pwd[64] = "56Afx1Tfr447e";
  char access_token[64] = "xtmJmf0JoDFEmMup2vv2";
  char mqtt_url[64] = "mqtt.monitoring-pmld.com";

  char ssid_tmp[64];
  char pwd_tmp[64];
  char access_token_tmp[64];
  char mqtt_url_tmp[64];


  flag_desc_id(BX_FLAG_ACTIVE);

  interval =  eeprom.read8(BASE_ADDR_INTERVAL_TIME);

  err_timing = timing_init(0);

  if(err_timing == BX_TIMING_ERR)
    Serial.println("[rtc] error occured! skip.");
  else
    Serial.println("[rtc] ok");

  err = eeprom_throw_page_value(hw_id, EEPROM_DATA_TYPE_HWID, EEPROM_DATA_NULL);
  ssid_len = eeprom.read8(BASE_ADDR_SSID_LEN);
  pwd_len = eeprom.read8(BASE_ADDR_PWD_LEN);
  mqtt_url_len = eeprom.read8(BASE_ADDR_MQTT_URL_LEN);

  if(err != BX_EEPROM_OK)
    Serial.printf("[eeprom] fail while try reading hardware id at 0x%d08\r\n", BASE_ADDR_HWID);
  else{
    Serial.printf("[eeprom] hardware id read from 0x%d08\r\n", BASE_ADDR_HWID);
    Serial.printf("[eeprom] hardware id : %s\r\n", hw_id);
  }

  Serial.println("[eeprom] sensor data will be sent every " + String(interval) + " mins");

#endif

  sensor_throw_sensor_value();
  timing_throw_value();

  bx_err_wifi err_wfi = wifi_init(ssid_len, pwd_len);

  if(err_wfi != BX_WIFI_CONNECTED){
    Serial.println("[wifi] error occured while trying to connect!");
    flag_update_desc_id = BX_SYS_MSG_ERR_NO_WIFI;
    flag_desc_id(BX_FLAG_ERR_WIFI);
  }else{
    Serial.println("[wifi] connected");
    Serial.println("[wifi] addr : " + WiFi.localIP().toString());
    flag_desc_id(BX_FLAG_OK);
  }

  mqtt_init(mqtt_url_len);

  bx_err_mqtt err_mqtt;
  err_mqtt = mqtt_publish(hw_id);

  if(err_mqtt == BX_MQTT_ERR){
    Serial.println("[mqtt] error while trying to publish!");
    flag_update_desc_id = BX_SYS_MSG_ERR_NO_CONN;
    flag_desc_id(BX_FLAG_ERR_WIFI);
  }else{
    Serial.println("[mqtt] published!");
    flag_update_desc_id = BX_SYS_MSG_OK;
    flag_desc_id(BX_FLAG_OK);
  }

  flag_deinit();
  system_sleep();

  /*** TASK LOOP RUN HERE BEGIN ***/
  /***  TASK LOOP RUN HERE END  ***/
}

void loop(){}
