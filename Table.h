#pragma once

#include "Robot.h"
#define PLACE_FOR_ALGO 13
#define PLACE_FOR_HOUSE 10
#define SEPRATE_CHAR "|"

class Table
{
	int n, m;
	vector<string> houseNames;
	vector<string> algoNames;
	vector<Robot*> robots;

	inline int placeForLine() {
		return PLACE_FOR_HOUSE*(m + 1) + PLACE_FOR_ALGO + m + 3;
	}
public:
	void printEmptyLine();
	void printHouseNamesLine();
	void printTable();
	Table(vector<string> houseNames1, vector<string> algoNames1, vector<Robot*> robots1) : houseNames(houseNames1),
		algoNames(algoNames1), robots(robots1) {
		n = algoNames.size();
		m = houseNames.size();
	}
	~Table();
};

