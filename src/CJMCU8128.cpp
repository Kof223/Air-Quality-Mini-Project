#include "Arduino.h"
#include "CJMCU8128.h"

// Initializes all the sensors on CJMCU8128
// Wire should be started before this is called
void CJMCU8128::begin()
{
    bme.settings.commInterface = I2C_MODE;
    bme.settings.I2CAddress = BME280Address;
    bme.settings.runMode = 3; //Normal mode
    
    bme.begin();
    hdc.begin(HDC1080Address);
    ccs.begin();
}

// Read the values from the sensors and updates it to data array
void CJMCU8128::update()
{
    if (ccs.dataAvailable())
    {
        ccs.readAlgorithmResults();
        Serial.println(hdc.readT());
        Serial.println(hdc.readH());
        // Temp and Humiditiy (Celsius and RH%)
        data[0] = hdc.readT() - 3.5; // These constants are for compensating for board heating
        data[1] = hdc.readH() + 3.8; // A sepearte board would be better

        // Air pressure (hpa)
        bme.readTempC();                         // Need this to get proper reading
        data[2] = bme.readFloatPressure() / 100; // Converted from pascals to hectopascal

        // CO2 and TVOC
        data[3] = ccs.getCO2();
        data[4] = ccs.getTVOC();

        // Compensating the CCS811 with humidity and temperature readings from the HDC1080
        ccs.setEnvironmentalData(hdc.readH(), hdc.readT());
    }
}

// Gets temperature data in celsius 
double CJMCU8128::getTemperature()
{
    return data[0];
}

// Gets humidity data
double CJMCU8128::getHumidity()
{
    return data[1];
}

// Gets atmospheric pressure in hpa
double CJMCU8128::getPressure()
{
    return data[2];
}

// Gets eCO2 in ppm
double CJMCU8128::getCO2()
{
    return data[3];
}

// Gets TVOC in ppb
double CJMCU8128::getTVOC()
{
    return data[4];
}
