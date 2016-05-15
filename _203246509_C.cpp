#include "_203246509_C.h"



Direction _203246509_C::step(Direction prevStep) {
	return SharedAlgorithm::chooseStep(1, prevStep);


}


//extern "C" AbstractAlgorithm* maker() {
//	return new _203246509_C();
//}

REGISTER_ALGORITHM(_203246509_C)