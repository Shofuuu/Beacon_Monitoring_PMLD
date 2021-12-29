#ifndef WSEN_HIDS_CPP
#define WSEN_HIDS_CPP

#include <stdint.h>

#include "WSEN_HIDS_C.h"

class Sensor_HIDS
{

public:
    
int init(int address)
{
    return I2CInit(address);
}

/**
   @brief   Return the device ID for this sensor
   @retval  Value of DEVICE_ID
*/

int get_DeviceID()
{
    uint8_t devID;

    if (HIDS_getDeviceID(&devID) == WE_SUCCESS)
    {
        return devID;
    }
    return WE_FAIL;
}

/**
   @brief  Set the output data rate 
   @retval Error Code
*/
int ODR;
int select_ODR()
{
    switch (ODR)
    {
        case 0:
        {
            HIDS_setOdr(oneShot);
            break;
        }

        case 1:
        {
            HIDS_setOdr(odr1HZ);
            break;
        }

        case 2:
        {
            HIDS_setOdr(odr7HZ);
            break;
        }

        case 3:
        {
            HIDS_setOdr(odr12_5HZ);
            break;
        }

        default:
        {
            return WE_FAIL;
        }
    }
    return WE_SUCCESS;

}


/*get the temperature and Humidity Output data  */

/**
 * @brief  Read the raw humidity output
 * @param  no parameter.
 * @retval Humidity Output
 */
uint16_t get_H_T_out()
{
	uint16_t H_T_out = 0;
	H_T_out= HIDS_get_H_T_out();
	return H_T_out;
}

/**
 * @brief  Read the raw temperature output
 * @param  no parameter.
 * @retval Temperature Output
 */
uint16_t get_T_OUT()
{
	uint16_t T_out = 0;
	T_out= HIDS_get_T_OUT();
	return T_out;
}

/*get the calibration parameters */

/**
 * @brief  Read the H0_T0_out calibration
 * @param  no parameter.
 * @retval H0_T0_out calibration
 */
 
uint16_t get_H0_T0_out()
{
	uint16_t H0_T0_out = 0;
	H0_T0_out= HIDS_get_H0_T0_out();
	return H0_T0_out;
}

/**
 * @brief  Read the H1_T0_out calibration
 * @param  no parameter.
 * @retval H1_T0_out calibration
 */
 
uint16_t get_H1_T0_out()
{
	uint16_t H1_T0_out = 0;
	H1_T0_out= HIDS_get_H1_T0_out();
	return H1_T0_out;
}


/**
 * @brief  Read the H0_rh calibration
 * @param  no parameter.
 * @retval H0_rh calibration
 */
 
uint16_t get_H0_rh()
{
	uint16_t H0_rh = 0;
	H0_rh= HIDS_get_H0_rh();
	return H0_rh;
}

/**
 * @brief  Read the H1_rh calibration
 * @param  no parameter.
 * @retval H1_rh calibration
 */
 
uint16_t get_H1_rh()
{
	uint16_t H1_rh = 0;
	H1_rh= HIDS_get_H1_rh();
	return H1_rh;
}
/**
 * @brief  Read the T1_OUT calibration
 * @param  no parameter.
 * @retval T1_OUT calibration
 */
 
uint16_t get_T1_OUT()
{
	uint16_t T1_OUT = 0;
	T1_OUT= HIDS_get_T1_OUT();
	return T1_OUT;
}
/**
 * @brief  Read the T0_OUT calibration
 * @param  no parameter.
 * @retval T0_OUT calibration
 */
 
uint16_t get_T0_OUT()
{
	uint16_t T0_OUT = 0;
	T0_OUT= HIDS_get_T0_OUT();
	return T0_OUT;
}
/**
* @brief Read the temperature Data availability
* @param  Pointer to the temperature Data availability state
* @retval Error code
*/
int8_t get_TempStatus()
{
	HIDS_state_t temp_state ;
	HIDS_getTempStatus(&temp_state);
	return temp_state ;
}
/**
* @brief Read the Humidity Data availability
* @param  Pointer to the Humidity Data availability state
* @retval Error code
*/
int8_t get_HumStatus() 
{
	HIDS_state_t humidity_state;
	HIDS_getHumStatus(&humidity_state);
	return humidity_state ;
}

/**
 * @brief  Read the Humidity data
 * @param  no parameter.
 * @retval error code
 */
float get_Humidity()
{
	float humidity ;
	HIDS_getHumidity(&humidity);
	return humidity ;
}

/**
 * @brief  Read the Temperature data
 * @param  no parameter.
 * @retval Temperature in °C
 */
float get_Temperature()
{
	float temperature ;
	HIDS_getTemperature(&temperature);
	return temperature ;
}

/**
 * @brief  Set the sensor to single conversion mode
 * @param  no parameter.
 * @retval none
 */
void set_single_conversion()
{
    HIDS_state_t oneShot = HIDS_enable;
    ODR = 0;
    select_ODR();
    HIDS_enOneShot(oneShot);
}

/**
 * @brief  Set the sensor to continuous mode
 * @param  no parameter.
 * @retval none
 */   
void set_continuous_mode(int outputDataRate)
{
    //Enable block data update
    HIDS_setBdu(HIDS_enable);
    ODR = outputDataRate;
    select_ODR();
    HIDS_setPowerMode(activeMode);
}

};

#endif