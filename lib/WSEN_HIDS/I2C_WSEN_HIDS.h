#ifndef I2C_WSEN_HIDS
#define I2C_WSEN_HIDS

#define TIMEOUT 1000

/**         Includes         **/

#include <stdint.h>
#include <stdlib.h>

#include <Arduino.h>
#include <stdbool.h>
#include <Wire.h>

#define WE_SUCCESS 0
#define WE_FAIL -1

int deviceAddress = 0;

/**
 * @brief  Initialize the I2C Interface
 * @param  I2C address
 * @retval Error Code
 */

int I2CInit(int address)
{

    deviceAddress = address;
    Wire.begin();
    return WE_SUCCESS;
}

/**
 * @brief  Set I2C bus Address
 * @param  I2C address
 * @retval Set Address
 */

void I2CSetAddress(int address) 
{ 
	deviceAddress = address; 
}

/**
 * @brief   Read data starting from the addressed register
 * @param  -RegAdr : register address to read from
 *         -NumByteToRead : number of bytes to read
 *         -pointer Data : address stores the data
 * @retval Error Code
 */

int ReadReg(uint8_t RegAdr, int NumByteToRead, uint8_t *Data)
{

    Wire.beginTransmission(deviceAddress);
    Wire.write(RegAdr);
    unsigned long start = millis();
    while (Wire.endTransmission() && millis() - start <= TIMEOUT);
    int n = Wire.requestFrom(deviceAddress, NumByteToRead);
    for (int i = 0; i < n; i++)
    {
        Data[i] = Wire.read();
    }

    return WE_SUCCESS;
}

/**
 * @brief  Write data starting from the addressed register
 * @param  -RegAdr : address to write in
 *         -NumByteToWrite : number of bytes to write
 *         -pointer Data : address of the data to be written
 * @retval Error Code
 */

int WriteReg(uint8_t RegAdr, int NumByteToWrite, uint8_t *Data)
{

    Wire.beginTransmission(deviceAddress);
    Wire.write(RegAdr);
 
    int n = Wire.requestFrom(deviceAddress, NumByteToWrite);
    for (int i = 0; i < n; i++)
    {
        Data[i] = Wire.write(*Data);
    }
    
   if (Wire.endTransmission())
    {
        return WE_FAIL;
    }

    return WE_SUCCESS;
}

 /**
 * @brief   Read data starting from the addressed register
 * @param  -pointer Data : the address store the data
 *         -NumByteToRead : number of bytes to read     
 * @retval Error Code
 */

int I2C_read(uint8_t *data, int bytesToRead)
{
	Wire.beginTransmission(deviceAddress);
    Wire.requestFrom(deviceAddress, bytesToRead); // request Bytes

	for(int index = 0; index < bytesToRead; index++)
	{
		data[index] = Wire.read();
	}
    
    return WE_SUCCESS;
}

#endif