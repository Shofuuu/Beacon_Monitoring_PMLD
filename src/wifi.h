#ifndef WIFI_H
#define WIFI_H

#include <WiFi.h>
#include "bx_err.h"

bx_err_wifi wifi_init(uint8_t ssid_len, uint8_t pwd_len){
    char ssid[64];
    char pwd[64];
    String pwd_tmp;
    char hwid[14];
    uint16_t timeout = 0;

    // eeprom_throw_page_value(ssid, EEPROM_DATA_TYPE_SSID, ssid_len);
    eeprom.readIntoMemArray((unsigned char*)ssid, BASE_ADDR_SSID, sizeof(ssid));
    // eeprom_throw_page_value(pwd, EEPROM_DATA_TYPE_PWD, pwd_len);
    eeprom.readIntoMemArray((unsigned char*)pwd, BASE_ADDR_PWD, sizeof(pwd));
    eeprom_throw_page_value(hwid, EEPROM_DATA_TYPE_HWID, EEPROM_DATA_NULL);

    for(uint8_t x=13;x<pwd_len;x++)
        pwd_tmp += String(pwd[x]);

    WiFi.setHostname((String("bxa-") + String(hwid)).c_str());

    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin((const char*)ssid, (const char*)pwd_tmp.c_str());
    Serial.println("[wifi] ssid : " + String(ssid) + ", pwd : " + String(pwd_tmp));

    timeout = millis();
    while(WiFi.status() != WL_CONNECTED){
        if((millis() - timeout) > 6000)
            return BX_WIFI_DISCONNECTED;
    }

    return BX_WIFI_CONNECTED;
}

#endif