#pragma once
#ifndef SCORE_H_
#define SCORE_H_


#include <map>
#include <string>
using namespace std;
class Score {
	int position;
	int winnerNumSteps;
	int numSteps;
	int dirtCollected;
	int sumDirtInHouse;
	int isBackInDocking; //1 is true, 0 is false
	int simulation_steps;
public:
	Score();
	virtual ~Score();
	int calcResult();   
	void createMapForScore(map<string, int> & mapForScore);
	//int calc_score(map<string, int> mapForScore);
	int getDirtCollected() const {
		return dirtCollected;
	}

	void setDirtCollected(int dirtCollected) {
		this->dirtCollected = dirtCollected;
	}

	int isIsBackInDocking() const {
		return isBackInDocking;
	}

	void setIsBackInDocking(int isBackInDocking) {
		this->isBackInDocking = isBackInDocking;
	}

	int getNumSteps() const {
		return numSteps;
	}

	void setNumSteps(int numSteps) {
		this->numSteps = numSteps;
	}

	int getPosition() const {
		return position;
	}

	void setPosition(int position) {
		this->position = position;
	}

	int getSumDirtInHouse() const {
		return sumDirtInHouse;
	}

	void setSumDirtInHouse(int sumDirtInHouse) {
		this->sumDirtInHouse = sumDirtInHouse;
	}

	int getWinnerNumSteps() const {
		return winnerNumSteps;
	}

	void setWinnerNumSteps(int winnerNumSteps) {
		this->winnerNumSteps = winnerNumSteps;
	}
};

#endif /* SCORE_H_ */

