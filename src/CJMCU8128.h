/*
  CJMCU8128.h - Library for CJMCU8128 Sensor Combo.
  Created by kof223, December 9, 2020.
*/
#ifndef CJMCU8128_h
#define CJMCU8128_h

#include "Arduino.h"
#include <SparkFunCCS811.h>     //Click here to get the library: http://librarymanager/All#SparkFun_CCS811
#include <SparkFunBME280.h>     //Click here to get the library: http://librarymanager/All#SparkFun_BME280
#include <ClosedCube_HDC1080.h> //Click here to get the library: http://librarymanager/All#ClosedCube_HDC1080

class CJMCU8128
{
private:
    const int CCS811Address = 0x5A;
    const int HDC1080Address = 0x40;
    const int BME280Address = 0x76;
    CCS811 ccs;
    ClosedCube_HDC1080 hdc;
    BME280 bme;
    double data[5];

public:
    CJMCU8128() : ccs(CCS811Address)
    {
    }
    void begin();
    void update();
    double getTemperature();
    double getHumidity();
    double getPressure();
    double getCO2();
    double getTVOC();
};

#endif
