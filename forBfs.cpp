#include "forBfs.h"


forBfs::forBfs(Point _currPoint, Point _parentPoint, int _dist_from_root)
{

	currPoint = _currPoint;
	parentPoint = _parentPoint;
	dist_from_root = _dist_from_root;





}


forBfs::~forBfs()
{

}

Point forBfs::getCurrPoint() const {
	return currPoint;
}

Point forBfs::getParentPoint() const {
	return parentPoint;
}

int forBfs::getDist_from_root() const {
	return dist_from_root;
}

void forBfs::setCurrPoint(Point curr) {
	currPoint = curr;
}

void forBfs::setParentPoint(Point parent) {
	parentPoint = parent;
}

void forBfs::setDist_from_root(int dist) {
	dist_from_root = dist;
}

void forBfs::setNode(Point curr, Point parent, int dist) {
	setCurrPoint(curr);
	setParentPoint(parent);
	setDist_from_root(dist);
}