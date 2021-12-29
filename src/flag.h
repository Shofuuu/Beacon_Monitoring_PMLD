#ifndef FLAG_H
#define FLAG_H

#define BX_FLAG_LED_R_PIN   GPIO_NUM_4
#define BX_FLAG_LED_G_PIN   GPIO_NUM_5
#define BX_FLAG_LED_B_PIN   GPIO_NUM_18

static const gpio_num_t flag_pin_map[3] = {
    BX_FLAG_LED_R_PIN,
    BX_FLAG_LED_G_PIN,
    BX_FLAG_LED_B_PIN
};

static void flag_init(){
    for(uint8_t x=0;x<3;x++){
        gpio_set_direction(flag_pin_map[x], GPIO_MODE_OUTPUT);
        // pinMode(flag_pin_map[x],OUTPUT);
    }
    for(uint8_t x=0;x<3;x++){
        gpio_set_level(flag_pin_map[x], 1);
        // digitalWrite(flag_pin_map[x],1);
    }
}

static void flag_desc_id(bx_err_flag flag){
    uint8_t rgb_val[3] = {1,1,1};

    switch(flag){
        case BX_FLAG_OK:
            rgb_val[0] = 1;
            rgb_val[1] = 0;
            rgb_val[2] = 1;
        break;
        case BX_FLAG_ERR_CONFIG:
            rgb_val[0] = 0;
            rgb_val[1] = 0;
            rgb_val[2] = 1;
        break;
        case BX_FLAG_CONFIG_MODE:
            rgb_val[0] = 1;
            rgb_val[1] = 1;
            rgb_val[2] = 0;
        break;
        case BX_FLAG_ACTIVE:
            rgb_val[0] = 0;
            rgb_val[1] = 0;
            rgb_val[2] = 0;
        break;
        case BX_FLAG_ERR_WIFI:
            rgb_val[0] = 0;
            rgb_val[1] = 1;
            rgb_val[2] = 1;
        break;
        case BX_FLAG_CONFIG_OK:
            rgb_val[0] = 0;
            rgb_val[1] = 1;
            rgb_val[2] = 0;
        break;

        default:
            rgb_val[0] = 1;
            rgb_val[1] = 1;
            rgb_val[2] = 1;
        break;
    }

    for(uint8_t x=0;x<3;x++)
        gpio_set_level(flag_pin_map[x], rgb_val[x]);
        
        
    Serial.printf("[flag] %d:%d:%d\r\n", rgb_val[0], rgb_val[1], rgb_val[2]);
    vTaskDelay(1000/portTICK_RATE_MS);
}

static bx_err_msg_sys flag_update_desc_id;

static String flag_throw_desc_msg(){
    String msg[4] = {
        "OK",
        "NO_WIFI",
        "NO_CONN",
        "ERR_SYS"
    };
    String tmp_cpy = msg[flag_update_desc_id];

    return tmp_cpy;
}

static void flag_deinit(){
    for(uint8_t x=0;x<3;x++){
        gpio_set_level(flag_pin_map[x],1);
        // gpio_set_direction(flag_pin_map[x], GPIO_MODE_INPUT);
    }
}
#endif