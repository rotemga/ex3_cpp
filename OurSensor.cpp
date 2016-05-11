#include "OurSensor.h"


OurSensor::OurSensor(House *house, Point* point) : thisHouse(house), currPoint(point)
{
	getInfoFromPoint();
}

OurSensor::OurSensor()
{
}
OurSensor::~OurSensor()
{
}


OurSensor::OurSensor(const OurSensor& sensor2) {//copy c'tor
	*this = sensor2;
}

OurSensor& OurSensor::operator=(const OurSensor& sensor2) {// '=' operator
	if (this != &sensor2) {
		setInfo(sensor2.sensorInfo);
	}
	return *this;
}





void OurSensor::setInfo(const SensorInformation& Info) {//update the sesnor with new information
	sensorInfo.dirtLevel = Info.dirtLevel;
	for (size_t i = 0; i < 4; ++i) {
		sensorInfo.isWall[i] = Info.isWall[i];
	}
}



SensorInformation OurSensor::sense() const {
	return this->sensorInfo;
}

void OurSensor::getInfoFromPoint() {
	//SensorInformation tmp;
	thisHouse->getInfo(*currPoint, sensorInfo);
	//setInfo(tmp);
}
void OurSensor::getInfoFromPoint(House* house, Point* point) {
	house->getInfo(*point, sensorInfo);
}