#include "_203246509_B.h"



Direction _203246509_B::step(Direction prevStep) {
	return SharedAlgorithm::chooseStep(0.5, prevStep);

}

//extern "C" AbstractAlgorithm* maker() {
//	return new _203246509_B();
//}

REGISTER_ALGORITHM(_203246509_B)