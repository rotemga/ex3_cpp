#include "Battery.h"
#include <algorithm>
Battery::Battery(int capacity, int conRate, int rachRate) :
capacity(capacity), conRate(conRate), rachRate(rachRate), currentState(capacity){
}

Battery::~Battery() {
	// TODO Auto-generated destructor stub
}
Battery::Battery(const Battery& battery2){//copy c'tor
	*this = battery2;
}

Battery& Battery::operator=(const Battery& battery2){// '=' operator
	this->capacity = battery2.capacity;
	this->conRate = battery2.conRate;
	this->currentState = battery2.currentState;
	this->rachRate = battery2.rachRate;
	return *this;
}


void Battery::recharge(int times) {
	currentState = std::min(capacity, currentState + times*rachRate);
}

bool Battery::isEmpry() {
	return currentState == 0;
}

bool Battery::isFull() {
	return currentState == capacity;
}

void Battery::reduce(int times) {
	currentState = std::max(0, currentState - times*conRate);
}