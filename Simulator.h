#pragma once
#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include <iostream>
#include <vector>
#include <map>
#include <time.h>
#include <stdlib.h> 
#include <algorithm>
#include "House.h"
#include "Score.h"
#include "AbstractAlgorithm.h"
#include "AbstractSensor.h"
#include "OurSensor.h"
#include "Robot.h"
#include "Table.h"
#include "Battery.h"
#include "Point.h"
using namespace std;
#define MAX_STEPS_AFTER_WINNER "MaxStepsAfterWinner"
#define BATTERY_CON_RATE "BatteryConsumptionRate"
#define BATTERY_CAPACITY "BatteryCapacity"
#define BATTERY_RECHARGE_RATE "BatteryRechargeRate"

class Simulator {
	vector <House*> houses;
	vector <AbstractAlgorithm*> algorithms;
	map<string, int> config;
	//vector<Score> results;

	//void updatePointByDirection(Point& point, Direction direction);
public:
	void run(vector<string> algoNames,vector<string> houseNames);
	void fillHouses(const string& location);
	void setInputHouses(vector<House*> input);
	void setInputConfig(map<string, int> input_config);
	void setInputAlgo(vector <AbstractAlgorithm*> input_algorithms);
	void createScore(int winner_num_stepsint, int num_steps, int pos_in_comeptition, bool is_back_docking, int dirt_collected, int sum_Dirth_House, Score *score);
	Simulator(vector <House*>, vector <AbstractAlgorithm*>);
	Simulator(vector <House*>, vector <AbstractAlgorithm*>, map<string, int>);
	bool allRobotsFinished(vector <Robot*> robots);
	bool robotWin(Robot* robot);
	virtual ~Simulator();

};

#endif /* SIMULATOR_H_ */