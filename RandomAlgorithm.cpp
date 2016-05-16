#include "RandomAlgorithm.h"


RandomAlgorithm::RandomAlgorithm()
{
	thisSensor = new OurSensor();
}


RandomAlgorithm::~RandomAlgorithm()
{
	delete thisSensor;
}

void RandomAlgorithm::setSensor(const AbstractSensor& sensor){
	SensorInformation a = sensor.sense();
	thisSensor->setInfo(a);
}

void RandomAlgorithm::setConfiguration(map<string, int> config){
	thisConfig = config;

	/*map<string, int>::iterator it;

	for (it = config.begin(); it != config.end(); it++)
	{
		thisConfig[it->first] = it->second;
	
	}*/

}
Direction RandomAlgorithm::step(){
	set<int> s;
	checkLegalDirection(s);
	//srand(time(NULL));
	int randIndex = rand() % s.size();
	set<int>::iterator it = s.begin();
	advance(it, randIndex);
	int stepIndex = *it;
	return static_cast<Direction>(stepIndex);

	/*Direction direct;
	while (1){
		int randIndex = rand() % 5; // randIndex in the range 0 to 4
		direct = static_cast<Direction>(randIndex);
		if (directionIsLegal(direct))
			break;
	}
	return direct;*/
}


void RandomAlgorithm::aboutToFinish(int stepsTillFinishing){

}

string RandomAlgorithm::ToString(const Direction direct){
	switch (direct)
	{
	case Direction::East:		return "East";
	case Direction::West:		return "West";
	case Direction::South:		return "South";
	case Direction::North:		return "North";
	case Direction::Stay:		return "Stay";
	default:					return "Unknown";
	}
}



//return true if the direction is legal.
//i.e. the direction is not lead to wall, or if the direction is stay, there is dust in the current location.
//index 0 is east, 1 is west, 2 is south, 3 is north
bool RandomAlgorithm::directionIsLegal(const Direction direct){
	SensorInformation info = thisSensor->sense();
	switch (direct)
	{
	case Direction::East:		return !(info.isWall[0]);
	case Direction::West:		return !(info.isWall[1]);
	case Direction::South:		return !(info.isWall[2]);
	case Direction::North:		return !(info.isWall[3]);
	case Direction::Stay:		return true; /*(info.dirtLevel > 0)*/
	default:					return false;
	}

}

//index 0 is east, 1 is west, 2 is south, 3 is north, 4 is stay

void RandomAlgorithm::checkLegalDirection(set<int>& s){
	SensorInformation info = thisSensor->sense();
	for (int i = 0; i < 4; ++i){
		if (!(info.isWall[i]))
			s.insert(i);
	}
	//if (info.dirtLevel > 0)
	s.insert(4);

}
