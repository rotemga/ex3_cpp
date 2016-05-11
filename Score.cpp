#include "Score.h"
#include <algorithm>
Score::Score() :
position(0), winnerNumSteps(0), numSteps(0), dirtCollected(0), sumDirtInHouse(
0), isBackInDocking(0), simulation_steps(0){

}

Score::~Score() {
}

int Score::calcResult() {

	return std::max(0,
		(2000 - (position - 1) * 50 + (winnerNumSteps - numSteps) * 10
		- (sumDirtInHouse - dirtCollected) * 3
		+ ((isBackInDocking==1) ? 50 : -200)));
}
