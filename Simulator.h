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
#include <atomic>
#include <thread>
#include <mutex>
typedef int(*scoreFunc)(const map<string, int>&);
using namespace std;
#define MAX_STEPS_AFTER_WINNER "MaxStepsAfterWinner"
#define BATTERY_CON_RATE "BatteryConsumptionRate"
#define BATTERY_CAPACITY "BatteryCapacity"
#define BATTERY_RECHARGE_RATE "BatteryRechargeRate"

class Simulator {
	vector <House*> houses;
	vector <AbstractAlgorithm*> algorithms;
	map<string, int> config;
	vector<Robot*> robots;
	atomic_int houseIndex{ 0 };
	mutex print_lock;
	mutex lock1;
	size_t num_of_algos;
public:
	void initAlgos(vector <AbstractAlgorithm*> algos);
	bool runSimulator(vector<string> algoNames, vector<string> houseNames,int threadNumber,scoreFunc scorefunc);
	void runMultiThreaded(size_t num_threads);
	void runSingleSubSimulationThread(int thread_num);
	void run();
	void setInputHouses(vector<House*> input);
	void setInputConfig(map<string, int> input_config);
	void setInputAlgo(vector <AbstractAlgorithm*> input_algorithms);
	void createScore(int winner_num_stepsint, int num_steps, int pos_in_comeptition, bool is_back_docking, int dirt_collected, int sum_Dirth_House, Score *score);
	Simulator(vector <House*>, vector <AbstractAlgorithm*>);
	Simulator(vector <House*>, vector <AbstractAlgorithm*>, map<string, int>);
	bool allRobotsFinished(int i,int j);
	bool robotWin(Robot* robot);
	void runOnOneHouse(int index_house, int thread_index);
	virtual ~Simulator();

};

#endif /* SIMULATOR_H_ */