#include "Score.h"

Score::Score() :
position(0), winnerNumSteps(0), numSteps(0), dirtCollected(0), sumDirtInHouse(
0), isBackInDocking(0), simulation_steps(0){

}

Score::~Score() {
}

//int Score :: calc_score(map<string,int> mapForScore) {
//	return Score::calcResult();
//}



int Score::calcResult() {

	return std::max(0,
		(2000 - (position - 1) * 50 + (winnerNumSteps - numSteps) * 10
		- (sumDirtInHouse - dirtCollected) * 3
		+ ((isBackInDocking==1) ? 50 : -200)));
}

void Score::createMapForScore(map<string, int> & mapForScore) {
	mapForScore["actual_position_in_competition"] = position;
	mapForScore["winner_num_steps"] = winnerNumSteps;
	mapForScore["this_num_steps"] = numSteps;
	mapForScore["dirt_collected"] = dirtCollected;
	mapForScore["sum_dirt_in_house"] = sumDirtInHouse;
	mapForScore["is_back_in_docking"] = isBackInDocking;
	mapForScore["simulation_steps"] = simulation_steps;

}
