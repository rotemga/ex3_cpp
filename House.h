#ifndef HOUSE_H_
#define HOUSE_H_
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "SensorInformation.h"
#include "Point.h"
using namespace std;

typedef enum { DIRTY, CLEAN, WALL, DOCKING } pointState;

typedef enum { GOOD_HOUSE, NO_DOCKING, MORE_THAN_ONE_DOCKING } houseState;



class House {
	int maxSteps;
	int R;
	int C;
	string houseDes;
	//string longDes;
	string* matrix;


public:
	House();
	House(const House&);//copy c'tor
	House& operator=(const House&);// '=' operator
	House* newCopyOfHouse();
	void cleanDirtyPoint(const Point& point);
	pointState findPointState(const Point& point) const;
	int currentValue(int x, int y) const;
	int findDockingStation(Point&) const;
	char& findDirtLevel(int x, int y) const;
	houseState checkIfHouseLegal(Point& point);
	virtual ~House();
	void fillHouseData(const string& filename, string& msg);
	void completeMissingBlanks();
	void eliminateStrangeChar();
	void putWallsOnSides();
	int sumDirt() const;
	void getInfo(Point& currPoint, SensorInformation& info) const;
	bool isClean() const;
	void output();


	int getmaxSteps() const {
		return maxSteps;
	}

	int getC() const {
		return C;
	}

	void setC(int c) {
		C = c;
	}

	string* getMatrix() const {
		return matrix;
	}


	int getR() const {
		return R;
	}

	void setR(int r) {
		R = r;
	}
};

#endif /* HOUSE_H_ */