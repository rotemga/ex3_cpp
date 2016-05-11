#pragma once
#ifndef FORBFS_H_
#define FORBFS_H_
#include "Point.h"
class forBfs
{
	Point currPoint;
	Point parentPoint;
	int dist_from_root;
public:
	forBfs(Point _currPoint, Point _parentPoint, int _dist_from_root);
	~forBfs();

	Point getCurrPoint() const;
	Point getParentPoint() const;
	int getDist_from_root() const;
	void setCurrPoint(Point curr);
	void setParentPoint(Point parent);
	void setDist_from_root(int dist);
	void setNode(Point curr, Point parent, int dist);


};

#endif /* FORBFS_H_ */