#include "_203246509_A.h"




Direction _203246509_A::step(Direction prevStep) {
	return SharedAlgorithm::chooseStep(5,prevStep);
}


//extern "C" AbstractAlgorithm* maker() {
//	return new _203246509_A();
//}


REGISTER_ALGORITHM(_203246509_A)
