#include "House.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include "SensorInformation.h"
using namespace std;




House::House() {
}

House::~House() {
	delete[] matrix;
}

House::House(const House& house2) {//copy c'tor
	*this = house2;
}

House& House::operator=(const House& house2) {// '=' operator
	int maxSteps = house2.maxSteps;
	int R = house2.getR();
	int C = house2.getC();
	string* newMatrix = new string[R];
	string* tmp = house2.getMatrix();
	for (int i = 0; i < R; ++i)
	{
		newMatrix[i].resize(C, 'X');
		for (int j = 0; j < C; ++j)
		{
			newMatrix[i][j] = tmp[i][j];
		}
	}
	House* newHouse = new House();
	newHouse->matrix = newMatrix;
	newHouse->maxSteps = maxSteps;
	newHouse->C = C;
	newHouse->R = R;
	newHouse->houseDes = house2.houseDes;

	//this->matrix = newMatrix;
	//this->maxSteps = maxSteps;
	//this->C = C;
	//this->R = R;
	//this->houseDes = house2.houseDes;
	return *newHouse;
}



House* House::newCopyOfHouse() {
	string* newMatrix = new string[R];
	for (int i = 0; i < R; ++i)
	{
		newMatrix[i].resize(C, 'X');
		for (int j = 0; j < C; ++j)
		{
			newMatrix[i][j] = matrix[i][j];
		}
	}
	House* newHouse = new House();
	newHouse->matrix = newMatrix;
	newHouse->maxSteps = maxSteps;
	newHouse->C = C;
	newHouse->R = R;
	newHouse->houseDes = houseDes;
	return newHouse;

}



int House::currentValue(int x, int y) const {
	return matrix[x][y];
}

bool House::isClean() const {
	for (int i = 0; i<R; i++)
		for (int j = 0; j<C; j++)
			if (matrix[i][j] >= '1' && matrix[i][j] <= '9')
				return false;
	return true;
}

int House::findDockingStation(Point& point) const {
	int counter = 0;
	for (int i = 0; i < R; i++)
		for (int j = 0; j < C; j++)
			if (matrix[i][j] == 'D') {
				point.setPoint(i, j);
				counter++;
			}
	return counter;
}

char& House::findDirtLevel(int x, int y) const {
	return matrix[x][y];
}
//if the house is llegal return true, else false.

houseState House::checkIfHouseLegal(Point& point) {
	completeMissingBlanks();
	eliminateStrangeChar();
	if (findDockingStation(point) == 0) {
		return NO_DOCKING;
	}
	putWallsOnSides();
	int numberOfDockingStations = findDockingStation(point);
	if (numberOfDockingStations == 0) {
		return NO_DOCKING;
	}
	else if (numberOfDockingStations > 1) {
		return MORE_THAN_ONE_DOCKING;
	}

	return GOOD_HOUSE;
}
/*stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int*/
inline bool isInteger(const std::string & s)
{
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

	char * p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
}

void House::fillHouseData(const string& filename, string& msg) {

	ifstream fin(filename);
	if (!fin) {
		msg = "cannot open file";
		return;
	}
	getline(fin, houseDes);
	string input;
	std::getline(fin, input);
	if (!isInteger(input)) {
		msg = "line number 2 in house file shall be positive number, found: " + input;
		return;
	}
	maxSteps = stoi(input);
	if (maxSteps <= 0) {
		msg = "line number 2 in house file shall be positive number, found: " + input;
		return;
	}

	std::getline(fin, input);
	if (!isInteger(input)) {
		msg = "line number 3 in house file shall be a positive number, found: " + input;
		return;
	}
	R = stoi(input);
	if (R <= 0) {
		msg = "line number 3 in house file shall be positive number, found: " + input;
		return;
	}
	std::getline(fin, input);
	if (!isInteger(input)) {
		msg = "line number 4 in house file shall be a positive number, found: " + input;
		return;
	}
	C = stoi(input);
	if (C <= 0) {
		msg = "line number 4 in house file shall be positive number, found: " + input;
		return;
	}
	matrix = new string[R];
	for (int i = 0; i < R; ++i) {
		matrix[i].resize(C, 'X');
		string buffer;
		std::getline(fin, buffer); //stackoverflow.com/questions/2251433/checking-for-eof-in-stringgetline
		int numOfCharsToFill = std::min(buffer.size(), matrix[i].size());
		for (int j = 0; j < numOfCharsToFill; j++)
			matrix[i][j] = buffer[j];
	}
	msg = "";
}

void House::completeMissingBlanks() {
	for (int i = 0; i < R; i++) {
		if ((int)matrix[i].size() < C)
			for (int j = matrix[i].size(); j < C; j++)
			{

				matrix[i][j] = ' ';
			}

	}
}

void House::eliminateStrangeChar() {
	for (int i = 0; i < R; i++)
		for (int j = 0; j < C; j++) {
			char ch = matrix[i][j];
			if (ch != 'W' && ch != 'D' && (ch < '1' || ch > '9'))
				matrix[i][j] = ' ';
		}
}

void House::putWallsOnSides() {
	for (int i = 0; i < C; ++i) {
		matrix[0][i] = 'W';
		matrix[R - 1][i] = 'W';
	}
	for (int i = 0; i < R; ++i) {
		matrix[i][0] = 'W';
		matrix[i][C - 1] = 'W';
	}
}

int House::sumDirt() const {
	int sum = 0;
	for (int i = 0; i < R; i++)
		for (int j = 0; j < C; j++)
			if (matrix[i][j] >= '1' && matrix[i][j] <= '9')
				sum += (int)matrix[i][j] - '0';
	return sum;
}
void House::cleanDirtyPoint(const Point& point) {
	matrix[point.getX()][point.getY()]--;
}
pointState House::findPointState(const Point& point) const {
	int x = point.getX();
	int y = point.getY();
	char ch = matrix[x][y];
	switch (ch) {
	case 'D':
		return DOCKING;
	case ' ':
		return CLEAN;
	case '0':
		return CLEAN;
	case 'W':
		return WALL;
	default:
		return DIRTY;
	}
}

//index 0 is east, 1 is west, 2 is south, 3 is north
void House::getInfo(Point& currPoint, SensorInformation& info) const {
	int x = currPoint.getX();
	int y = currPoint.getY();
	if (matrix[x][y] == 'D' || matrix[x][y] == 'W' || matrix[x][y] == ' ') {
		info.dirtLevel = 0;
	}
	else {
		info.dirtLevel = matrix[x][y] - '0';
	}

	info.isWall[0] = (y == getC() - 1) ? true : matrix[x][y + 1] == 'W';
	info.isWall[1] = (y == 0) ? true : matrix[x][y - 1] == 'W';
	info.isWall[2] = (x == 0) ? true : matrix[x + 1][y] == 'W';
	info.isWall[3] = (x == getR() - 1) ? true : matrix[x - 1][y] == 'W';
}

void House::output()
{
	cout << "Printing house from instance into standard output" << endl;
	//	cout << "House name: " << shortDes << endl;
	//	cout << "House description: " << longDes << endl;
	for (int i = 0; i < R; ++i)
	{
		for (int j = 0; j < C; ++j)
		{
			cout << matrix[i][j];
		}
		cout << endl;
	}
	cout << endl;
}