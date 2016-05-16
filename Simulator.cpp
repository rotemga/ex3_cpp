#include "Simulator.h"
#include "make_unique.h"
#include "AlgorithmRegistration.h"
#include "AlgorithmRegistrar.h"


void Simulator:: runMultiThreaded(size_t num_threads) {
	// ===> create the threads as vector of pointers to threads (unique_ptr)
	vector<unique_ptr<thread>> threads(num_threads);
	if (num_threads > 1){
		int cnt = num_threads - 1;
		while (cnt > 0){
			vector<unique_ptr<AbstractAlgorithm>> Algos;
			Algos = AlgorithmRegistrar::getInstance().getAlgorithms();
			for (vector<unique_ptr<AbstractAlgorithm>>::size_type k = 0; k != Algos.size(); k++) {
				algorithms.push_back(Algos[k].release());
			}
			cnt--;
		}
	}
	initAlgos(algorithms);
	cout << "num of algos= " << algorithms.size() << endl;


	int i = 0;
	for (auto& thread_ptr : threads) {
		// ===> actually create the threads and run them
		thread_ptr = make_unique<thread>(&Simulator::runSingleSubSimulationThread,this,i); // create and run the thread
		i++;
	}

	// ===> join all the threads to finish nicely (i.e. without crashing / terminating threads)
	for (auto& thread_ptr : threads) {
		thread_ptr->join();
	}
}
void Simulator :: runSingleSubSimulationThread(int thread_num) {
	// ===> thread should take a new task, if available, and run it
	// if no task is available, thread is done


	for (size_t index = houseIndex++; // fetch old value, then add. equivalent to: fetch_add(1)
	index < houses.size();
		index = houseIndex++) {
		//lock_guard<mutex> lock(print_lock);
		cout << "house number : " << index 
			<< " from thread #" << this_thread::get_id() << endl;
		Simulator::runOnOneHouse(index,thread_num);
	}

}

Simulator::Simulator(vector <House*> houses1, vector <AbstractAlgorithm*> algos1)
{
	houses = houses1;
	algorithms = algos1;
	num_of_algos = algos1.size();
	//robots = vector<Robot*> (num_of_algos*houses.size());
	for (size_t i = 0; i < num_of_algos*houses.size(); i++){
		robots.push_back(nullptr);
	}
	cout << "ctor robot.size=" << robots.size() << endl;
}
Simulator::Simulator(vector <House*> houses1, vector <AbstractAlgorithm*> algos1, map<string, int> config1) :
houses(houses1), algorithms(algos1), config(config1), num_of_algos(algos1.size()) {
	for (size_t i = 0; i < num_of_algos*houses.size(); i++){
		robots.push_back(nullptr);
	}
	cout << "ctor robot.size=" << robots.size() << endl;
	
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
	num_of_algos = input_algorithms.size();
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
bool Simulator::allRobotsFinished(int i,int j) {
	 int cnt = 0;
	for (int k = i; k != j; k++) {
		if (!(robots[k]->isCanRun()))
			cnt++;
	}
	if (cnt == j-i)
		return true;
	return false;
}
//return true if the robot win (i.e it's finished cleaning the house and it's back to docking station)
bool Simulator::robotWin(Robot* robot) {
	return ((robot->isHouseClean()) && (robot->areWeInDocking()));
}

void Simulator::runOnOneHouse(int index_house, int thread_index) {
	Point* point = new Point(-1, -1);
	Score score;
	createScore(0, 0, 0, false, 0, 0, &score);
	int Steps = houses[index_house]->getmaxSteps(), pos_in_competition = 1, actual_position_in_copmetition = 1, sum_dirt = houses[index_house]->sumDirt();
	int simulation_steps = 0, winner_num_steps = 0, num_of_wins_this_iter = 0;
	int aboutToFinish = 0;
	int algo_index_in_algorithms_vector = thread_index*num_of_algos;

	houses[index_house]->findDockingStation(*point);

	//{
	//	mutex l;
	//	lock_guard<mutex> lock(l);
		for (vector<AbstractAlgorithm*>::size_type j = 0; j != num_of_algos; j++) {
			// robot for all pair <house,algorithm>
			House* tmp_house = houses[index_house]->newCopyOfHouse();
			Point* tmp_point = point->newCopyOfPoint();
			Battery* battery = new Battery(config.at(BATTERY_CAPACITY), config.at(BATTERY_CON_RATE), config.at(BATTERY_RECHARGE_RATE));
			Robot* robot = new Robot(tmp_house, algorithms[algo_index_in_algorithms_vector + j], tmp_point, battery);
			robots[num_of_algos*index_house + j] = robot;
			cout << "robot init index=" << j*index_house + j << endl;
		}


	houses[index_house]->output();
	cout << "index_house = " << index_house << endl; 
	while (Steps > 0) {//each iteration simulate each robot one step
		for (vector<Robot*>::size_type k = index_house*num_of_algos; k != (index_house*num_of_algos + num_of_algos); k++) {
			//cout << "robotNUM= " << k << endl;
			//cout << "robotSize = " << robots.size() << endl;
			if (!robots[k]->isCanRun())
				continue;

			robots[k]->runRobot();



			if (robotWin(robots[k])) {//robot win, update Steps and it's score 
				if (aboutToFinish == 0) {//call aboutToFinish of all algos unless the function updated MAX_STEPS_AFTER_WINNER before.
					aboutToFinish = 1;
					if (config.at(MAX_STEPS_AFTER_WINNER) > houses[index_house]->getmaxSteps()) {
						Steps = min(config.at(MAX_STEPS_AFTER_WINNER), (houses[index_house]->getmaxSteps() - simulation_steps)) + 1;
					}
					else {
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
			if (houses[index_house]->getmaxSteps() - config.at(MAX_STEPS_AFTER_WINNER) == simulation_steps) {
				if (aboutToFinish == 0) {
					aboutToFinish = 1;
					Steps = config.at(MAX_STEPS_AFTER_WINNER) + 1;
				}
			}
		}

		if (1) {
			lock_guard<mutex> lock(lock1);
			if (allRobotsFinished(index_house*num_of_algos, index_house*num_of_algos + num_of_algos /*robots.size()*/))//check if there are robots that can run
				break;
		}

		if (aboutToFinish == 1) {//update all algorithms with MAX_STEPS_AFTER_WINNER
			aboutToFinish = -1;
			for (vector<AbstractAlgorithm*>::size_type j = 0; j != num_of_algos; j++) {
				if (config.at(MAX_STEPS_AFTER_WINNER) > houses[index_house]->getmaxSteps()) {
					algorithms[algo_index_in_algorithms_vector + j]->aboutToFinish(min(config.at(MAX_STEPS_AFTER_WINNER), (houses[index_house]->getmaxSteps() - simulation_steps) + 1));
				}
				else {
					algorithms[algo_index_in_algorithms_vector + j]->aboutToFinish(config.at(MAX_STEPS_AFTER_WINNER));
				}

			}
		}
		if (num_of_wins_this_iter > 0) {
			actual_position_in_copmetition += num_of_wins_this_iter;
		}
		num_of_wins_this_iter = 0;
		if (simulation_steps >= houses[index_house]->getmaxSteps())
			break;
		else {
			simulation_steps++;
			Steps--;
		}
	}
	if (winner_num_steps == 0) {
		winner_num_steps = simulation_steps;
	}	

	for (vector<Robot*>::size_type k = index_house*num_of_algos; k != index_house*num_of_algos + num_of_algos; k++) {
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


void Simulator::initAlgos(vector <AbstractAlgorithm*> algos) {
	for (vector<AbstractAlgorithm*>::size_type i = 0; i != algos.size(); i++) {
		algos[i]->setConfiguration(config);
	}
}

bool Simulator::runSimulator(vector<string> algoNames, vector<string> houseNames,int threadNumber,scoreFunc scorefunc) {
	//initAlgos();
	(threadNumber) ?  runMultiThreaded(threadNumber) : run();
	Table table(houseNames, algoNames, robots);
	bool flag=table.printTable(scorefunc);
	for (auto it = robots.begin(); it != robots.end(); ++it) {
		delete *it;
	}
	robots.clear();
	return flag;

}

//simulate the running of the simulator
void Simulator::run() {
	initAlgos(algorithms);
	for (vector<House*>::size_type i = 0; i != houses.size(); i++) {
		runOnOneHouse(i,0);
	}
	/*Table table(houseNames, algoNames, robots);
	table.printTable();
	for (auto it = robots.begin(); it != robots.end(); ++it) {
		delete *it;
	}
	robots.clear();*/
}


