#include "SharedAlgorithm.h"


SharedAlgorithm::SharedAlgorithm()
{
	currPoint = new Point(0, 0);//docking station is in (0,0)
	targetPoint = new Point(0, 0);
	wanted_point = new Point(0, 0);
	StepsTillFinishing = -1;
	num_of_steps = 0;
	cnt = 0;
	last_direct = Direction::Stay;

}


SharedAlgorithm::~SharedAlgorithm()
{
	delete currPoint;
	delete targetPoint;
	delete wanted_point;

	map_info.clear();
	moves.clear();
}


void SharedAlgorithm::setSensor(const AbstractSensor& sensor) {
	map_info.clear();
	moves.clear();
	thisSensor = &sensor;
	//set configuration
	setConfiguration(thisConfig);
	StepsTillFinishing = -1;
	num_of_steps = 0;
	cnt = 0;
	currPoint->setPoint(0, 0);
	targetPoint->setPoint(0, 0);
	wanted_point->setPoint(0, 0);
	last_direct = Direction::Stay;





}

void SharedAlgorithm::setConfiguration(map<string, int> config) {
	thisConfig = config;
	CurrBattery = thisConfig.at("BatteryCapacity");
	ConRateBattery = thisConfig.at("BatteryConsumptionRate");
	BatteryRechargeRate = thisConfig.at("BatteryRechargeRate");
	BatteryCapacity = thisConfig.at("BatteryCapacity");

}
Direction SharedAlgorithm::step(Direction prevStep) {
	return Direction::Stay;
}


void SharedAlgorithm::aboutToFinish(int stepsTillFinishing) {
	StepsTillFinishing = stepsTillFinishing;
}







// '*' = we know it's not a wall
void SharedAlgorithm::updateMapInfo(SensorInformation info, Direction prevStep) {
	updateMapStep(prevStep);
	int x = currPoint->getX();
	int y = currPoint->getY();
	if (currPoint->isInDocking()) {
		map_info[Point(x, y)] = 'D';
	} //in docking
	else {
		int i = info.dirtLevel;
		if (i > 0) { i = i - 1; }
		char c = i + '0';
		map_info[Point(x, y)] = c;
	} //stay
	if (!map_info.count(Point(x, y + 1)))
		map_info[Point(x, y + 1)] = ((info.isWall[0]) ? 'W' : '*'); //east
	if (!map_info.count(Point(x, y - 1)))
		map_info[Point(x, y - 1)] = ((info.isWall[1]) ? 'W' : '*'); // west
	if (!map_info.count(Point(x + 1, y)))
		map_info[Point(x + 1, y)] = ((info.isWall[2]) ? 'W' : '*'); //south
	if (!map_info.count(Point(x - 1, y)))
		map_info[Point(x - 1, y)] = ((info.isWall[3]) ? 'W' : '*'); //north


}



//
////return the length of the minimal path from current point to docking.
//int SharedAlgorithm::findMinPathToDocking(bool first_iteraion, int cnt, int ok, const Point position, map<Point, int> thisMoves, int min) {
//
//	map<Point, int> m;
//	//int min = map_info.size();
//
//	if (position.isInDocking()) {
//		ok = 1;
//		return cnt;
//	}
//	if (ok == 1)
//		return cnt;
//	if (cnt >= min)
//		return cnt;
//	else {
//		legalMoves(m, 1, position);//update m to be only legal directions (no option to stay in place)
//		map<Point, int>::iterator it;
//		for (it = m.begin(); it != m.end(); it++) {//iterate over all legal moves
//			//if (first_iteraion){
//			//	thisMoves.clear();
//			//	cnt = 0;
//			//}
//			if (thisMoves.count(it->first) == 0){
//				thisMoves[it->first] = cnt;
//			}
//			else{
//				continue;
//			}
//
//
//			cnt = findMinPathToDocking(false, cnt+1, ok, it->first, thisMoves, min);
//
//			if (cnt < min) {
//				min = cnt;
//				if (ok == 1) return min;
//			}
//		}
//	}
//	return min;
//
//}






int SharedAlgorithm::getMaxCoordinate(map<Point, char> m, int coordinate) {
	int max = 0;
	for (auto iterator = m.begin(); iterator != m.end(); iterator++) {
		Point p1 = iterator->first;
		int value = (coordinate == 1) ? p1.getX() : p1.getY();
		max = (abs(value) > max) ? abs(value) : max;
	}
	return max;
}
char SharedAlgorithm::interputForPrint(char ch) {
	if (ch == 'D' || ch == 'W' || ch == '*' || (ch >= '0' && ch <= '9') || ch == ' ')
		return ch;
	return '?';
}

void SharedAlgorithm::printCurrMap(map <Point, char> m) {
	map<Point, char>::iterator it;
	int maxX = getMaxCoordinate(m, 1);
	int maxY = getMaxCoordinate(m, 2);
	for (int i = maxX; i >= -maxX; i--) {
		for (int j = -maxY; j <= maxY; j++)
			cout << interputForPrint(m[Point(-i, j)]);
		cout << "  y = " << i << endl;
	}
}

Direction SharedAlgorithm::get_Direction(Point* wanted_point, float score_for_star) {
	return Direction::Stay;
}




//get score_for_star and wanted_point as inputs
//update wanted_point to be best point to move to it.
Direction SharedAlgorithm::get_Move(Point* wanted_point, float score_for_star) {
	float max_score = -2, east = -2, west = -2, south = -2, north = -2, stay = 0;
	int x = currPoint->getX(), y = currPoint->getY();
	map<Point, int> m;
	legalMoves(m, 0, *currPoint);//update m to be only legal directions
	if (m.size() == 0)
		return  Direction::Stay;
	if (house_clean()) 	return bfs(*currPoint, Point(0, 0), wanted_point);

	if (StepsTillFinishing != -1) { //the function aboutToFinish called and updated StepsTillFinishing

		if (StepsTillFinishing*(ConRateBattery + 1)== CurrBattery) {//if we have enough steps, we must go back to docking
			return bfs(*currPoint, Point(0, 0), wanted_point);

		}

	}
	if (/*static_cast<int>(moves.size())*/num_of_steps*ConRateBattery + 1 >= CurrBattery) {//if we don't have enough battery, we must go back to docking
		return bfs(*currPoint, Point(0, 0), wanted_point);

	}
	if (currPoint->isInDocking() && CurrBattery < BatteryCapacity){
		wanted_point->setPoint(x, y);
		targetPoint->setPoint(*wanted_point);
		return Direction::Stay;
	}

	map<Point, int>::iterator it;
	for (it = m.begin(); it != m.end(); it++) {//iterate over all legal moves

		if ((it->first.getY() == y + 1) && ((it->first.getX() == x))) {//east
			east = ((it->second >= 0) ? it->second : ((it->second == -1) ? 0 : score_for_star));
		}
		else if ((it->first.getY() == y - 1) && ((it->first.getX() == x))) {//west
			west = ((it->second >= 0) ? it->second : ((it->second == -1) ? 0 : score_for_star));
		}
		else if ((it->first.getX() == x + 1) && ((it->first.getY() == y))) {//south
			south = ((it->second >= 0) ? it->second : ((it->second == -1) ? 0 : score_for_star));
		}
		else if ((it->first.getX() == x - 1) && ((it->first.getY() == y))) {//north
			north = ((it->second >= 0) ? it->second : ((it->second == -1) ? 0 : score_for_star));
		}
		else if ((it->first.getX() == x) && ((it->first.getY() == y))) {//stay
			stay = ((it->second >= 0) ? it->second : ((it->second == -1) ? 0 : score_for_star));
		}
	}
	num_of_steps++;
	max_score = std::max(std::max(east, west), std::max(std::max(south, north), stay));
	if (max_score == 0) {//if all legal direction with no dirt, check in which direction there are most stars
		return mostStars(wanted_point, score_for_star);
	}

	if (east == max_score) {
		wanted_point->setPoint(x, y + 1);
		targetPoint->setPoint(*wanted_point);
		return  Direction::East;
	}
	else if (west == max_score) {
		wanted_point->setPoint(x, y - 1);
		targetPoint->setPoint(*wanted_point);
		return  Direction::West;
	}
	else if (south == max_score) {
		wanted_point->setPoint(x + 1, y);
		targetPoint->setPoint(*wanted_point);
		return  Direction::South;
	}
	else if (north == max_score) {
		wanted_point->setPoint(x - 1, y);
		targetPoint->setPoint(*wanted_point);
		return  Direction::North;
	}
	else if (stay == max_score) {
		wanted_point->setPoint(x, y);
		targetPoint->setPoint(*wanted_point);
		num_of_steps--;
		return  Direction::Stay;
	}
	targetPoint->setPoint(*wanted_point);
	return Direction::Stay;

}

Direction SharedAlgorithm::mostStars(Point* wanted_point, float score_for_star) {
	int x = currPoint->getX(), y = currPoint->getY();
	bool ok_east = true, ok_west = true, ok_south = true, ok_north = true;
	float east = 0, west = 0, south = 0, north = 0;
	int tmp;
	map<Point, int> m;

	//int min_path = 0;
	//Direction last_move = moves[moves.size() - 1], reverse_last_move = reverseDirection(last_move, wanted_point);
	//if (checkIfAlgoStack()){
	//	if (reverse_last_move == Direction::East) ok_east = false;
	//	else if (reverse_last_move == Direction::West) ok_west = false;
	//	else if (reverse_last_move == Direction::South) ok_south = false;
	//	else if (reverse_last_move == Direction::North) ok_north = false; 


	//}
	if (!(*currPoint==*targetPoint))
		return bfs(*currPoint, *targetPoint, wanted_point);


	Point east_point, west_point, south_point, north_point;
	int dist1 = map_info.size(), dist2 = dist1, dist3 = dist1, dist4 = dist1;
	map<Point, char>::iterator it;
	int iter = 0;
	for (it = map_info.begin(); it != map_info.end(); it++) {


		tmp = charToDirtLevel(it->second);
		if ((it->first.getY() > y) && (tmp > 0 || tmp == -3) && (map_info[Point(x, y + 1)] != 'W') && ok_east) {
			east += ((tmp > 0) ? tmp : 2);
			if (iter == 0){
				east_point = it->first;
				dist1 = bfs_dist(*currPoint, east_point);
			}
			else if (bfs_dist(*currPoint, it->first) < dist1){
				east_point = it->first;
				dist1 = bfs_dist(*currPoint, it->first);
			}
		}
		tmp = charToDirtLevel(it->second);
		if ((it->first.getY() < y) && (tmp > 0 || tmp == -3) && (map_info[Point(x, y - 1)] != 'W') && ok_west) {
			west += ((tmp > 0) ? tmp : 2);
			if (iter == 0){
				west_point = it->first;
				dist2 = bfs_dist(*currPoint, west_point);
			}
			else if (bfs_dist(*currPoint, it->first) < dist2){
				west_point = it->first;
				dist2 = bfs_dist(*currPoint, it->first);
			}
		}
		tmp = charToDirtLevel(it->second);
		if ((it->first.getX() > x) && (tmp > 0 || tmp == -3) && (map_info[Point(x + 1, y)] != 'W') && ok_south) {
			south += ((tmp > 0) ? tmp : 2);
			
			if (iter == 0){
				south_point = it->first;
				dist3 = bfs_dist(*currPoint, south_point);
			}
			else if (bfs_dist(*currPoint, it->first) < dist3){
				south_point = it->first;
				dist3 = bfs_dist(*currPoint, it->first);
			}


		}
		tmp = charToDirtLevel(it->second);
		if ((it->first.getX() < x) && (tmp > 0 || tmp == -3) && (map_info[Point(x - 1, y)] != 'W') && ok_north) {
			north += ((tmp > 0) ? tmp : 2);

			if (iter == 0){
				north_point = it->first;
				dist4 = bfs_dist(*currPoint, north_point);
			}
			else if (bfs_dist(*currPoint, it->first) < dist4){
				north_point = it->first;
				dist4 = bfs_dist(*currPoint, it->first);
			}
		}
		iter++;



	}
	float max_score = std::max(std::max(east, west), std::max(south, north));
	if (max_score == 0) {
		return bfs(*currPoint, Point(0, 0), wanted_point);


	}
	if (east == max_score) {
		targetPoint->setPoint(east_point.getX(), east_point.getY());
		return bfs(*currPoint,east_point, wanted_point);

		//wanted_point->setPoint(x, y + 1);
		//return  Direction::East;
	}
	else if (west == max_score) {
		targetPoint->setPoint(west_point.getX(), west_point.getY());
		return bfs(*currPoint, west_point, wanted_point);

		//wanted_point->setPoint(x, y - 1);
		//return  Direction::West;
	}
	else if (south == max_score) {
		targetPoint->setPoint(south_point.getX(), south_point.getY());
		return bfs(*currPoint, south_point, wanted_point);

		//wanted_point->setPoint(x + 1, y);
		//return  Direction::South;
	}
	else if (north == max_score) {
		targetPoint->setPoint(north_point.getX(), north_point.getY());
		return bfs(*currPoint, north_point, wanted_point);

		//wanted_point->setPoint(x - 1, y);
		//return  Direction::North;
	}

	return Direction::Stay;
}

// get direction and wanted_point,
// it's reverse the direction and update the wanted_point to be the point is this direction
Direction SharedAlgorithm::reverseDirection(Direction direct, Point *wanted_point) {
	int x = currPoint->getX(), y = currPoint->getY();
	//cout << "Back to docking, currPoint: (" << x << " ," << y << ")" << endl;
	//moves.erase(moves.size() - 1);
	switch (direct) {
	case Direction::East:
		wanted_point->setPoint(x, y - 1);
		return Direction::West;
	case Direction::West:
		wanted_point->setPoint(x, y + 1);
		return Direction::East;
	case Direction::North:
		wanted_point->setPoint(x + 1, y);
		return Direction::South;
	case Direction::South:
		wanted_point->setPoint(x - 1, y);
		return Direction::North;
	default:
		return Direction::Stay;
	}
	return Direction::Stay;
}


//check if algorithm stack in loop. (for example: west east west east...)
bool SharedAlgorithm::checkIfAlgoStack() {
	if (moves.size() >= 3) {
		Direction last_direction = moves[moves.size() - 1];
		Direction second_last_direction = moves[moves.size() - 2];
		Direction third_last_direction = moves[moves.size() - 3];

		if (last_direction != third_last_direction)
			return false;
		Point* tmp = new Point(0, 0);
		if (reverseDirection(third_last_direction, tmp) == second_last_direction) {
			delete tmp;
			return true;
		}
		delete tmp;
	}
	return false;

}
//get point as input and return the direction it's provides
Direction SharedAlgorithm::changePointToDirection(Point newPoint) {
	int x_cur = currPoint->getX();
	int y_cur = currPoint->getY();
	int x_new = newPoint.getX();
	int y_new = newPoint.getY();

	if (x_cur == x_new) {
		if (y_cur == y_new) { return Direction::Stay; }
		return ((y_cur + 1 == y_new) ? Direction::East : Direction::West);
	}
	else {
		return ((x_cur + 1 == x_new) ? Direction::South : Direction::North);

	}


}
//get as input map and update it with legal moves (points as key and current information as int)
//-1 is docking, -2 is wall, -3 is not wall, 0-9 is dirt level.
void SharedAlgorithm::legalMoves(map<Point, int> &m, int for_min_path, const Point point) {
	//int x = currPoint->getX();
	//int y = currPoint->getY();
	int x = point.getX();
	int y = point.getY();
	int val;
	Point* tmp = new Point(x, y);
	if (!for_min_path) m[*currPoint] = charToDirtLevel(map_info[*currPoint]);//insert stay. for func findMinPathToDocking (int for_min_path==1) don't insert stay!
	tmp->setPoint(x, y + 1);
	if (map_info.count(*tmp)) {
		if ((val = charToDirtLevel(map_info[*tmp])) != -2)//East not wall
			m[*tmp] = val;
	}
	tmp->setPoint(x, y - 1);
	if (map_info.count(*tmp)) {
		if ((val = charToDirtLevel(map_info[*tmp])) != -2)//West not wall
			m[*tmp] = val;
	}
	tmp->setPoint(x + 1, y);
	if (map_info.count(*tmp)) {
		if ((val = charToDirtLevel(map_info[*tmp])) != -2)//South not wall 
			m[*tmp] = val;
	}
	tmp->setPoint(x - 1, y);
	if (map_info.count(*tmp)) {
		if ((val = charToDirtLevel(map_info[*tmp])) != -2)//North not wall
			m[*tmp] = val;
	}
	delete tmp;
}


//get char as input and return the value that represents it in int.
// -1 = docking
// -2 = wall
// -3 = we know it's not wall
// 0 - 9 = dirt level
int SharedAlgorithm::charToDirtLevel(char c) {
	if (c == 'W') { return -2; }// -2 is wall
	if (c == 'D') { return -1; }// -1 is docking
	int tmp = c - '0';
	return (c == '*') ? -3 : tmp; // -3 is '*' (not Wall, but dirtlevel is unknown), else 0-9 dirt level
}



//get score_for_star as input and return direction.
Direction SharedAlgorithm::chooseStep(float score_for_star, Direction prevStep) {
	SensorInformation a = thisSensor->sense();
	updateMapInfo(a,prevStep);
	//Point* wanted_point = new Point(0, 0);
	Direction direct = get_Move(wanted_point, score_for_star);
	last_direct = direct;
	//currPoint->setPoint(*wanted_point);

	//if (currPoint->isInDocking()) {
	//	if (CurrBattery < 0) {
	//		CurrBattery = 0;
	//	}
	//	CurrBattery += BatteryRechargeRate;
	//	num_of_steps = 0;
	//}
	//else {
	//	CurrBattery -= ConRateBattery;
	//}
	//cout << "currPoint: (" << currPoint->getX() << " ," << currPoint->getY() << ")" << endl;
	//printCurrMap(map_info);
	//if ((currPoint->getX() == 1) && (currPoint->getY() == 2)) {
	//	cout << "cnt: " << cnt << endl;
	//}
	//delete wanted_point;
	//cnt++;
	//if (direct != Direction::Stay){
	//	moves[moves.size()] = direct;
	//	//moves->insert(direct);
	//}
	return direct;
}

void SharedAlgorithm::updateMapStep(Direction prevStep){
	if (last_direct != prevStep){
		updateWantedPoint(prevStep);
	}
	currPoint->setPoint(*wanted_point);

	if (currPoint->isInDocking()) {
		if (CurrBattery < 0) {
			CurrBattery = 0;
		}
		if (CurrBattery < BatteryCapacity){
			CurrBattery += BatteryRechargeRate;
			num_of_steps = 0;
		}
	}
	else {
		CurrBattery -= ConRateBattery;
	}
	//cout << "currPoint: (" << currPoint->getX() << " ," << currPoint->getY() << ")" << endl;
	//printCurrMap(map_info);
	//if ((currPoint->getX() == 1) && (currPoint->getY() == 2)) {
	//	cout << "cnt: " << cnt << endl;
	//	}
	cnt++;
	if (prevStep != Direction::Stay){
		moves[moves.size()] = prevStep;
		}
	

}


void SharedAlgorithm::updateWantedPoint(Direction prevStep){
	int x = currPoint->getX(), y = currPoint->getY();
	if (last_direct != prevStep){
		switch (prevStep){
		case(Direction::East) :
			wanted_point->setPoint(x, y + 1);

		case (Direction::West) :
			wanted_point->setPoint(x, y - 1);
		case(Direction::South) :
			wanted_point->setPoint(x + 1, y);
		case (Direction::North) :
			wanted_point->setPoint(x - 1, y);

		case(Direction::Stay) :
			wanted_point->setPoint(x, y);

		}
	}
}


//return true if there isn't dirt or stars in house
bool SharedAlgorithm::thereIsNoDirtOrStarsInMap(){
	int tmp;

	map<Point, char>::iterator it;
	for (it = map_info.begin(); it != map_info.end(); it++) {
		tmp = charToDirtLevel(it->second);
		//we check if we found position that is not a wall, docking, or a position with no dust.
		//if we found position that it's dirtlevel > 0 or it's a star, we return false.
		if (tmp != -2 && tmp != -1 && tmp != 0){ 
			return false;
		}
	}
	return true;



}


//This function using bfs algorithm to find the best move to go to the target point.
//It's finding the minimal path from rootPoint to the target point.
// return the Point that is the first point in this path. (the point is a legal move from the rootPoint).
Direction SharedAlgorithm::bfs(Point rootPoint, Point target, Point* wanted_point){
	map<Point, int> m;
	set <Point> s;
	queue<forBfs> queue;
	forBfs root(rootPoint, rootPoint, 0);//this is the root, it's parent isn't matter. 
	queue.push(root);
	//forBfs thisNode , childNode;
	int min_dist = map_info.size();
	bool first_iter = true;
	Point parentPoint, bestPoint;
	if (rootPoint == target) return Direction::Stay;

	while (!queue.empty()){
		forBfs thisNode = queue.front();
		queue.pop();
		if (s.find(thisNode.getCurrPoint()) != s.end()){//if we check this point, we don't want to check it again.
			continue;
		}
		m.clear();
		legalMoves(m, 1, thisNode.getCurrPoint());//update m to be only legal directions
		map<Point, int>::iterator it;
		for (it = m.begin(); it != m.end(); it++) {
			parentPoint = (first_iter) ? it->first : thisNode.getParentPoint();
			if (s.find(it->first) != s.end()){//if we check this point, we don't want to check it again.
				continue;
			}
			forBfs childNode(it->first, parentPoint, thisNode.getDist_from_root() + 1);
			queue.push(childNode);
			if ((min_dist > childNode.getDist_from_root())&& (it->first==target)){
				min_dist = childNode.getDist_from_root();
				bestPoint = childNode.getParentPoint();
			}

		}


		first_iter = false;
		s.insert(thisNode.getCurrPoint());

		


	}
	wanted_point->setPoint(bestPoint);
	Direction direct = changePointToDirection(bestPoint);
	return direct;


}

//This function using bfs algorithm to find the best move to go to the target point.
//It's finding the minimal path from rootPoint to the target point.
// return the Point that is the first point in this path. (the point is a legal move from the rootPoint).
int SharedAlgorithm::bfs_dist(Point rootPoint, Point target){
	map<Point, int> m;
	set <Point> s;
	queue<forBfs> queue;
	forBfs root(rootPoint, rootPoint, 0);//this is the root, it's parent isn't matter. 
	queue.push(root);
	//forBfs * thisNode = nullptr, *childNode = nullptr;
	int min_dist = map_info.size();
	bool first_iter = true;
	Point parentPoint, bestPoint;
	if (rootPoint == target) return 0;

	while (!queue.empty()){
		forBfs thisNode = queue.front();
		queue.pop();
		if (s.find(thisNode.getCurrPoint()) != s.end()){//if we check this point, we don't want to check it again.
			continue;
		}
		m.clear();
		legalMoves(m, 1, thisNode.getCurrPoint());//update m to be only legal directions
		map<Point, int>::iterator it;
		for (it = m.begin(); it != m.end(); it++) {
			parentPoint = (first_iter) ? it->first : thisNode.getParentPoint();
			if (s.find(it->first) != s.end()){//if we check this point, we don't want to check it again.
				continue;
			}
			forBfs childNode(it->first, parentPoint, thisNode.getDist_from_root() + 1);
			queue.push(childNode);
			if ((min_dist > childNode.getDist_from_root()) && (it->first == target)){
				min_dist = childNode.getDist_from_root();
				bestPoint = childNode.getParentPoint();
			}

		}


		first_iter = false;
		s.insert(thisNode.getCurrPoint());




	}

	return min_dist;


}


bool SharedAlgorithm::house_clean(){
	map<Point, char>::iterator it;
	int tmp;
	for (it = map_info.begin(); it != map_info.end(); it++) {
		tmp = charToDirtLevel(it->second);
		if (!(tmp == 0 || tmp == -1 || tmp==-2))
			return false;

	}
	return true;
}