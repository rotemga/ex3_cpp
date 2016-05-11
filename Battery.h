#pragma once
#ifndef BATTERY_H_
#define BATTERY_H_

class Battery {
	int capacity;
	int conRate;
	int rachRate;
	int currentState;
public:
	Battery(int capacity, int conRate, int rachRate);
	virtual ~Battery();
	Battery(const Battery&);//copy c'tor
	Battery& operator=(const Battery&);// '=' operator


	bool isEmpry();
	bool isFull();
	void recharge(int times = 1);
	void reduce(int times = 1);

	int getCapacity() const {
		return capacity;
	}

	void setCapacity(int capacity) {
		this->capacity = capacity;
	}

	int getConRate() const {
		return conRate;
	}

	void setConRate(int conRate) {
		this->conRate = conRate;
	}

	int getRachRate() const {
		return rachRate;
	}

	void setRachRate(int rachRate) {
		this->rachRate = rachRate;
	}
};

#endif /* BATTERY_H_ */