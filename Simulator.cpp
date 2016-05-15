#include "Simulator.h"



Simulator::Simulator(vector <House*> houses1, vector <AbstractAlgorithm*> algos1)
{
	houses = houses1;
	algorithms = algos1;
}
Simulator::Simulator(vector <House*> houses1, vector <AbstractAlgorithm*> algos1, map<string, int> config1) :
	houses(houses1), algorithms(algos1), config(config1) {

}



Simulator::~Simulator() {
}

void Simulator::setInputHouses(vector<House*> input_houses) {
	houses = input_houses;
}
void Simulator::setInputConfig(map<string, int> input_config) {
	config = input_config;
}
void Simulator::setInputAlgo(vector <AbstractAlgorithm*> input_algorithms) {
	algorithms = input_algorithms;
}


//update the input score with all the parameters
void Simulator::createScore(int winner_num_stepsint, int num_steps, int pos_in_comeptition, bool is_back_docking, int dirt_collected, int sum_Dirth_House, Score *score) {
	score->setWinnerNumSteps(winner_num_stepsint);
	score->setNumSteps(num_steps);
	score->setIsBackInDocking(is_back_docking);
	score->setDirtCollected(dirt_collected);
	score->setPosition(pos_in_comeptition);
	score->setSumDirtInHouse(sum_Dirth_House);
}




//This function gets as input vector of robots and check if all the robots finished running
bool Simulator::allRobotsFinished(vector <Robot*> robots) {
	unsigned int cnt = 0;
	for (vector<Robot*>::size_type k = 0; k != robots.size(); k++) {
		if (!(robots[k]->isCanRun()))
			cnt++;
	}
	if (cnt == robots.size())
		return true;
	return false;
}
//return true if the robot win (i.e it's finished cleaning the house and it's back to docking station)
bool Simulator::robotWin(Robot* robot) {
	return ((robot->isHouseClean()) && (robot->areWeInDocking()));
}


void PrintTable(vector<Robot*> robots, vector<string> houseNames, vector<string> algoNames) {

}
//simulate the running of the simulator
void Simulator::run(vector<string> algoNames,vector<string> houseNames) {

	vector <Robot*> robots;
	Score score;
	//int aboutToFinish = 0;

	for (vector<AbstractAlgorithm*>::size_type i = 0; i != algorithms.size(); i++) {
		algorithms[i]->setConfiguration(config);
	}



	for (vector<House*>::size_type i = 0; i != houses.size(); i++) {
		Point* point = new Point(-1, -1);
 
		createScore(0, 0, 0, false, 0, 0, &score);
		int Steps = houses[i]->getmaxSteps(), pos_in_competition = 1, actual_position_in_copmetition = 1,sum_dirt = houses[i]->sumDirt();
		int simulation_steps = 0, winner_num_steps = 0, num_of_wins_this_iter = 0;
		int aboutToFinish = 0;

		houses[i]->findDockingStation(*point);
		for (vector<AbstractAlgorithm*>::size_type j = 0; j != algorithms.size(); j++) {
			// robot for all pair <house,algorithm>
			House* tmp_house = houses[i]->newCopyOfHouse();
			Point* tmp_point = point->newCopyOfPoint();
			Battery* battery = new Battery(config.at(BATTERY_CAPACITY), config.at(BATTERY_CON_RATE), config.at(BATTERY_RECHARGE_RATE));
			Robot* robot = new Robot(tmp_house, algorithms[j], tmp_point, battery);
			robots.push_back(robot);
		}
		houses[i]->output();
		while (Steps > 0) {//each iteration simulate each robot one step
			for (vector<Robot*>::size_type k = i*algorithms.size(); k != robots.size(); k++) {
				if (!robots[k]->isCanRun())
					continue;

				robots[k]->runRobot();

				if (robotWin(robots[k])) {//robot win, update Steps and it's score 
					if (aboutToFinish == 0) {//call aboutToFinish of all algos unless the function updated MAX_STEPS_AFTER_WINNER before.
						aboutToFinish = 1;
						if (config.at(MAX_STEPS_AFTER_WINNER) > houses[i]->getmaxSteps()){
							Steps = min(config.at(MAX_STEPS_AFTER_WINNER), (houses[i]->getmaxSteps() - simulation_steps)) + 1;
						}
						else{
							Steps = config.at(MAX_STEPS_AFTER_WINNER) + 1;
						}
					}
					if (winner_num_steps == 0) {
						winner_num_steps = simulation_steps;
					}
					num_of_wins_this_iter++;
					pos_in_competition = min(4, actual_position_in_copmetition);
					createScore(winner_num_steps, simulation_steps, pos_in_competition, true, robots[k]->DirtCollected(), sum_dirt, &score);
					cout << "(winner_num_steps, simulation_steps, pos_in_competition, true, robots[k]->DirtCollected(), sum_dirt)" << winner_num_steps << " " << simulation_steps << endl;
					robots[k]->setCanRun(false);
					robots[k]->setScore(score);



				}
				if (houses[i]->getmaxSteps() - config.at(MAX_STEPS_AFTER_WINNER) == simulation_steps) {
					if (aboutToFinish == 0) {
						aboutToFinish = 1;

						Steps = config.at(MAX_STEPS_AFTER_WINNER) + 1;
						
					}

				}

			}
			if (allRobotsFinished(robots))//check if there are robots that can run
				break;

			if (aboutToFinish == 1) {//update all algorithms with MAX_STEPS_AFTER_WINNER
				aboutToFinish = -1;
				for (vector<AbstractAlgorithm*>::size_type j = 0; j != algorithms.size(); j++){
					if (config.at(MAX_STEPS_AFTER_WINNER) > houses[i]->getmaxSteps()){
						algorithms[j]->aboutToFinish(min(config.at(MAX_STEPS_AFTER_WINNER), (houses[i]->getmaxSteps() - simulation_steps) + 1));

					}
					else{
						algorithms[j]->aboutToFinish(config.at(MAX_STEPS_AFTER_WINNER));
					}
				}
			}
			if (num_of_wins_this_iter > 0) {
				actual_position_in_copmetition += num_of_wins_this_iter;
			}
			num_of_wins_this_iter = 0;

			if (simulation_steps >= houses[i]->getmaxSteps())
				break;
			else{
				simulation_steps++;
				Steps--;
			}

		}


		if (winner_num_steps == 0) {
			winner_num_steps = simulation_steps;
		}



		for (vector<Robot*>::size_type k = i*algorithms.size(); k != robots.size(); k++) {
			//the robot didn't win:
			//the robot crashed a wall or the battery is empty 
			//or:
			//the robot  didn't finish cleaning the house and it can run, but it's steps finished
			if ((robots[k]->isCanRun() || robots[k]->isBrokedDown())) {
			//	int tmp = robots[k]->DirtCollected();
				
				createScore(winner_num_steps, simulation_steps, 10, robots[k]->areWeInDocking(), robots[k]->DirtCollected(), sum_dirt, &score);
				cout << " not winning (winner_num_steps, simulation_steps, pos_in_competition, true, robots[k]->DirtCollected(), sum_dirt)" << winner_num_steps << " " << simulation_steps << endl;

				cout << score.calcResult() << endl;
				robots[k]->setScore(score);

			}

		}
	delete point;



	}



	Table table(houseNames, algoNames, robots);
	table.printTable();

	for (auto it = robots.begin(); it != robots.end(); ++it) {
		delete *it;
	}

	//for (auto it = algorithms.begin(); it != algorithms.end(); ++it) {
	//	delete *it;
	//}

	robots.clear();
	//algorithms.clear();


}