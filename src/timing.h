#ifndef TIMING_H
#define TIMING_H

#include <RTClib.h>

struct{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
}timing_t;

RTC_DS3231 rtc_ds3231;
uint8_t interval_data;

bx_err_timing timing_init(uint8_t calibrate){

    interval_data = eeprom.read8(BASE_ADDR_INTERVAL_TIME);

    if(!rtc_ds3231.begin())
        return BX_TIMING_ERR;

    if(calibrate)
        rtc_ds3231.adjust(DateTime(F(__DATE__), F(__TIME__)));

    rtc_ds3231.disable32K();
    rtc_ds3231.clearAlarm(1);
    rtc_ds3231.clearAlarm(2);
    rtc_ds3231.writeSqwPinMode(DS3231_OFF);
    rtc_ds3231.disableAlarm(2);

    if(rtc_ds3231.alarmFired(1)){
        rtc_ds3231.clearAlarm(1);
        Serial.println("[alarm] flag 0x01 cleared");
    }

    if(!rtc_ds3231.setAlarm1(rtc_ds3231.now() + TimeSpan(0,0,interval_data,0), DS3231_A1_Minute))
        Serial.println("[alarm] not set!");
    else
        Serial.println("[alarm] new alarm armed");

    return BX_TIMING_OK;
}

static void timing_throw_value(void){
    DateTime now = rtc_ds3231.now();
    timing_t.year = now.year();
    timing_t.month = now.month();
    timing_t.day = now.day();
    timing_t.hour = now.hour();
    timing_t.minute = now.minute();
    timing_t.second = now.second();
}

static void timing_month_to_str(char str_time[], uint8_t index){
    const char *month[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char str_month[5];

    strncpy(str_month, month[index], sizeof(month[index]));
    for(uint8_t x=0;x<strlen(str_time);x++)
        str_time[x] = str_month[x];
}

static uint64_t timing_epoch_ms_now(){
    DateTime t_now = rtc_ds3231.now();
    // uint64_t epochtime_ms = (t_now.secondstime() + 946684800) * 1000;
    uint64_t epochtime_ms = (t_now.secondstime() + 946684800) - (3600*7);

    return epochtime_ms;
}

#endif