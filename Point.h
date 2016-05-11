#pragma once
#ifndef POINT_H_
#define POINT_H_
#include "Direction.h"
#include <iostream>
#include <functional>
using namespace std;
class Point {
	int x;
	int y;
public:
	Point(int _x, int _y);
	Point();
	bool isInDocking() const;
	void print(ostream& out = cout) const {
		out << "(" << x << "," << y << ")" << endl;
	}
	void setPoint(int _x, int _y) {
		x = _x;
		y = _y;
	}

	void setPoint(Point p) {
		x = p.getX();
		y = p.getY();
	}
	void move(Direction d) {

		switch (d) {
		case Direction::East:
			y++;
			break; 
		case Direction::West:
			y--;
			break;
		case Direction::South:
			x++;
			break;
		case Direction::North:
			x--;
			break;
		case Direction::Stay:
			break;
		default:;
		}
	}
	friend ostream& operator<<(ostream& out, const Point& p) {
		p.print(out);
		return out;
	}

	int getX() const {
		return x;
	}

	int getY() const {
		return y;
	}


	bool operator<(const Point& point)const;
	bool operator==(const Point& point)const;
	Point* newCopyOfPoint();
};

#endif /* POINT_H_ */