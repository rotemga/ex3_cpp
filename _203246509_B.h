#pragma once
#ifndef _203246509_B_H_
#define _203246509_B_H_
#include "SharedAlgorithm.h"
class _203246509_B :
	public SharedAlgorithm
{

public:
	virtual Direction step(Direction prevStep) override;

};

#endif /* _203246509_B_H_ */