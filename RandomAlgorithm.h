#pragma once


#ifndef RANDOMALGORITHM_H_
#define RANDOMALGORITHM_H_
using namespace std;

#include "AbstractAlgorithm.h"
#include "OurSensor.h"
#include <string>
#include <map>
#include <stdlib.h> 
#include <set>
#include <time.h>

class RandomAlgorithm : public  AbstractAlgorithm {

	OurSensor* thisSensor;
	map<string, int> thisConfig;

public:
	RandomAlgorithm();
	virtual ~RandomAlgorithm();

	//Blocking Copy and Assignment
	RandomAlgorithm(const RandomAlgorithm&) = delete;
	RandomAlgorithm& operator=(const RandomAlgorithm&) = delete;

	virtual void setSensor(const AbstractSensor& sensor) override;
	virtual void setConfiguration(map<string, int> config) override;
	virtual Direction step() override;
	virtual void aboutToFinish(int stepsTillFinishing) override;

	bool directionIsLegal(const Direction direct);
	void checkLegalDirection(set<int>& s);
	string ToString(const Direction direct);

};

#endif /* RANDOMALGORITHM_H_ */

