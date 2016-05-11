#include "Robot.h"

Robot::Robot(House *house, AbstractAlgorithm *algo, Point* docking,
	Battery* battery) :
	house(house), algo(algo), position(docking), battery(battery) {
	sensor = new OurSensor(house, docking);
	score.setSumDirtInHouse(house->sumDirt());
	score.setPosition(10);
	canRun = true;
	brokedDown = false;
	algo->setSensor(*sensor);
	prevStep = Direction::Stay;
}

Robot::~Robot() {
	delete sensor;
	delete battery;
	delete(house);
	delete(position);
}

void Robot::runRobot() {
	if (canRun) {
		if (updateHouse(*position))
			score.setDirtCollected(score.getDirtCollected() + 1);
		updateBattery(*position, *battery);
		if (battery->isEmpry()) {
			canRun = false;
			brokedDown = true;
			score.setPosition(10);
			return;
		}
		//	algo->setSensor(*sensor);

		Direction direction = algo->step(prevStep); //should check if direction is legal
		prevStep = direction;
		//cout << "Direction step is: " << DirectionToString(direction) << endl;
		position->move(direction);
		sensor->setPoint(position);

		if (crashedToWall(*position)) {
			canRun = false;
			brokedDown = true;
			return;
		}
		//house->output();
		sensor->getInfoFromPoint(house, position);
		score.setNumSteps(score.getNumSteps() + 1);
	}
}

bool Robot::updateHouse(const Point& point) {
	if (house->findPointState(point) == DIRTY) {
		house->cleanDirtyPoint(point);
		return true;
	}
	return false;
}
void Robot::updateBattery(Point& point, Battery& battery) {
	switch (house->findPointState(point)) {
	case DOCKING:
		battery.recharge();
		break;
	default:
		battery.reduce();
	}
}

void Robot::updatePositionDirt(Point& point) {
	int dirt = house->currentValue(point.getX(), point.getY());
	if (dirt > 0) {
		house->cleanDirtyPoint(point);
	}
}

bool Robot::isHouseClean() const {
	return house->isClean();
}
bool Robot::areWeInDocking() const {
	return house->findPointState(*position) == DOCKING;
}

int Robot::DirtCollected() {
	return (score.getSumDirtInHouse() - house->sumDirt());
}
int Robot::sumDirtInHouse() {
	return(house->sumDirt());
}

void Robot::printHouse() {
	string* matrix = house->getMatrix();
	cout << "Printing house from instance into standard output" << endl;
	for (int i = 0; i < house->getR(); ++i)
	{
		for (int j = 0; j < house->getC(); ++j)
		{
			cout << matrix[i][j];
		}
		cout << endl;
	}
	cout << endl;
}



string Robot::DirectionToString(Direction d) {
	switch (d) {
	case Direction::East:
		return "East";

	case Direction::West:
		return "West";

	case Direction::South:
		return "South";

	case Direction::North:
		return "North";

	case Direction::Stay:
		return "Stay";

	default: return "Stay";
	}
	return "Stay";
}