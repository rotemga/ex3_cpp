#ifndef SHAREDALGORITHM_H_
#define SHAREDALGORITHM_H_

#include "AbstractAlgorithm.h"
#include "AbstractSensor.h"
#include "Point.h"
#include "forBfs.h"
#include <algorithm>
#include <queue>  
#include <set>
#include "make_unique.h"
#include "AlgorithmRegistration.h"

using namespace std;
class SharedAlgorithm :
	public AbstractAlgorithm
{
	const AbstractSensor* thisSensor;
	map<string, int> thisConfig;
	map<Point, char> map_info;
	map<int, Direction> moves;
	Point *currPoint;
	int CurrBattery;
	int ConRateBattery;
	int BatteryRechargeRate;
	int BatteryCapacity;
	int StepsTillFinishing;
	int num_of_steps;
	int cnt;
	bool mostStarsCalled = false;
	Direction last_direct;

	Point* targetPoint;
	Point* wanted_point;



public:
	SharedAlgorithm();
	virtual ~SharedAlgorithm();
	virtual void setSensor(const AbstractSensor& sensor) override;
	virtual void setConfiguration(map<string, int> config) override;
	virtual Direction step(Direction prevStep);
	virtual void aboutToFinish(int stepsTillFinishing) override;

	void updateMapInfo(SensorInformation info, Direction prevStep);
	//void  updatePointDirt();

	//Direction directionToDocking(Point* wanted_point);
	//Direction forDirectionToDockingFunc(int x, int y, map <Point, int> m, Point* wanted_point);
	//bool canGoBackToDocking();
	//int SharedAlgorithm::findMinPathToDocking(bool first_iteraion, int cnt, int ok, const Point position, map<Point, int> thisMoves, int min);
	//void printMap(map<Point, int> m, Point point);
	int getMaxCoordinate(map<Point, char> m, int coordinate);
	char interputForPrint(char ch);
	void printCurrMap(map <Point, char> m);
	//char* DirectionToString(Direction d);
	Direction get_Move(Point* wanted_point, float score_for_star);
	Direction mostStars(Point* wanted_point, float score_for_star);
	Direction changePointToDirection(Point newPoint);
	void legalMoves(map<Point, int> &m, int for_min_path, const Point point);
	int charToDirtLevel(char c);
	Direction chooseStep(float score_for_star, Direction prevStep);
	Direction reverseDirection(Direction direct, Point *wanted_point);
	bool checkIfAlgoStack();
	bool thereIsNoDirtOrStarsInMap();
	Direction bfs(Point rootPoint, Point target, Point* wanted_point);
	int bfs_dist(Point rootPoint, Point target);
	Direction get_Direction(Point* wanted_point, float score_for_star);
	bool house_clean();
	void updateMapStep(Direction prevStep);
	void updateWantedPoint(Direction prevStep);
};

#endif /* SHAREDALGORITHM_H_ */