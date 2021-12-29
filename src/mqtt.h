#ifndef MQTT_H
#define MQTT_H

#define MQTT_CLIENT_TOPIC       "v1/devices/me/telemetry"
#define MQTT_CLIENT_URL         "mqtt.conneximo.com"

#define MQTT_CLIENT_PASSWORD    ""
#define MQTT_CLIENT_PORT        1883

#include <PubSubClient.h>
#include <WiFiClient.h>

WiFiClient mqtt_wifi_client;
PubSubClient mqtt_pub_client(mqtt_wifi_client);

bx_err_mqtt mqtt_remove_char(char *s, char c){

    int writer = 0, reader = 0;

    while(s[reader]){
        if(s[reader]!=c){
            s[writer++] = s[reader];
        }
        reader++;
    }
    s[writer] = 0;
}

bx_err_mqtt mqtt_init(uint8_t mqtt_url_len){

    char mqtt_url[64];
    String mqtt_url_tmp;

    eeprom.readIntoMemArray((unsigned char*)mqtt_url, BASE_ADDR_MQTT_URL, sizeof(mqtt_url));

    for(uint8_t i=17; i<mqtt_url_len; i++)
        mqtt_url_tmp = mqtt_url_tmp + String(mqtt_url[i]);

    mqtt_remove_char(mqtt_url,'*');
    mqtt_remove_char(mqtt_url,'#');
    memmove(&mqtt_url[0],&mqtt_url[1],strlen(mqtt_url) - 0);
    
    Serial.println("[mqtt] URL is set to : " + String(mqtt_url));

    // mqtt_pub_client.setServer(MQTT_CLIENT_URL, MQTT_CLIENT_PORT);
    mqtt_pub_client.setServer((const char*)mqtt_url,MQTT_CLIENT_PORT);

    return BX_MQTT_OK;
}

bx_err_mqtt mqtt_publish(char hwid[14]){

    char access_token[128];
    char data_json[255];
    
    eeprom.readIntoMemArray((unsigned char*)access_token, BASE_ADDR_ACCES_TOKEN, sizeof(access_token));

    Serial.println("[mqtt] access token is set to : " + String(access_token));

    system_throw_json_format(data_json, hwid);
    mqtt_pub_client.setSocketTimeout(10000);

    Serial.println("[mqtt] json : " + String(data_json));

    if(mqtt_pub_client.connect(BX_DEVICE_TYPE, (const char*)access_token , MQTT_CLIENT_PASSWORD)){
        mqtt_pub_client.publish(MQTT_CLIENT_TOPIC, data_json, false);
        mqtt_pub_client.disconnect();
        return BX_MQTT_OK;
    }
    else
        return BX_MQTT_ERR;

}

#endif