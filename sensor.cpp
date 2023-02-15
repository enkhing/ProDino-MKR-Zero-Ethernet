#include "import.h"
#include "sensor.h"

void sensor::countForSensor() {
	sensorCount++;    
  Serial.println(sensorCount);
}

void sensor::setCountToZero() {
  sensorCount = 0;
}

int sensor::getCount() {
  return sensorCount;
}

int sensor::getMaxCount() {
  return maxSensorCount;
}
