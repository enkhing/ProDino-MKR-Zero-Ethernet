#include "import.h"

class sensor {
	public:
	
	const int maxSensorCount = 100;
	
	int sensorCount = 0;

	void countForSensor();
  void setCountToZero();
  int getCount();
  int getMaxCount();
};
