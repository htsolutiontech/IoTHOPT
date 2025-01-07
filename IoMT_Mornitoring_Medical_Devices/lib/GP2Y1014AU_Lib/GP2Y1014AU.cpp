/**************************************************************************/
/*!
@file     SharpGP2Y10.h
@author   lnquy065
@license  GNU GPLv3
@version  1.0
First version of an Arduino Library for the SharpGP2Y10 dust sensor
*/
/**************************************************************************/
#include <Arduino.h>
#include "GP2Y1014AU.h"

/**
 * Constructor
 * @param ledPin     Sensor infrared LED pin number
 * @param measurePin Sensor analog measure pin number
 * @return GP2Y1010AU0F object
 */
GP2Y1010AU0F::GP2Y1010AU0F(uint8_t ledPin, uint8_t measurePin) {
  this->_LEDPin = ledPin;
  this->_measurePin = measurePin;
}

/**
 * Setup sensor object
 * @return true when done
 */
bool GP2Y1010AU0F::begin() {
  pinMode(this->_LEDPin, OUTPUT);
  digitalWrite(this->_LEDPin, HIGH); // power off the LED
  return true;
}

/**
 * Simply read the current dust density.
 * @return Current dust density. Unit: ug/m3
 */
float GP2Y1010AU0F::read() {
  float voMeasured  = 0;
  float calcVoltage = 0;
  float dustDensity = 0;
  
  digitalWrite(this->_LEDPin, LOW); // power on the LED
  delayMicroseconds(this->_samplingTime);
  voMeasured = analogRead(this->_measurePin); // read the dust value
  delayMicroseconds(this->_deltaTime);
  digitalWrite(this->_LEDPin, HIGH); // turn the LED off
  delayMicroseconds(this->_sleepTime);

  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (this->_VCC / 4096.0);
  // linear equation taken from http://www.howmuchsnow.com/arduino/airquality/
  dustDensity = (0.17 * calcVoltage - 0.1) * 1000.0;
  return dustDensity;
}
