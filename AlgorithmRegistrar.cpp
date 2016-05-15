#include "AlgorithmRegistrar.h"


//AlgorithmRegistrar::AlgorithmRegistrar()
//{
//}


//AlgorithmRegistrar::~AlgorithmRegistrar()
//{
//}
// create the static field
AlgorithmRegistrar AlgorithmRegistrar::instance;

//----------------------------------------------------------------------------
// AlgorithmRegistrar::loadAlgorithm is a SUGGESTION only.
// You can implement something else instead. Of course the implementation
// below is a STUB implementation that do not actually load so files.
// returns:  0 for success
//          -1 if file cannot be loaded
//          -2 if file doesn't register any algorithm
// (To be in AlgorithmRegistrar.cpp which is part of the simulation project)
//----------------------------------------------------------------------------
int AlgorithmRegistrar::loadAlgorithm(const std::string& path, const std::string& so_file_name_without_so_suffix) {
	size_t size = instance.size();

	void* handler = dlopen(path.c_str(), RTLD_NOW);
	if (handler == nullptr) {
		return FILE_CANNOT_BE_LOADED;
	}

	if (instance.size() == size) {
		return NO_ALGORITHM_REGISTERED; // no algorithm registered
	}
	instance.setNameForLastAlgorithm(so_file_name_without_so_suffix);
	return ALGORITHM_REGISTERED_SUCCESSFULY;
	}



