#include <iostream>
#include <functional>
#include <memory>
#include <list>
#include <vector>
#include <cassert>
#include "AbstractAlgorithm.h"
#include <dlfcn.h>


class AlgorithmRegistrar {
	list<std::string> algorithmNames;
	list<std::function<unique_ptr<AbstractAlgorithm>()>> algorithmFactories;
	void registerAlgorithm(std::function<unique_ptr<AbstractAlgorithm>()> algorithmFactory) {
		instance.algorithmFactories.push_back(algorithmFactory);
	}
	void setNameForLastAlgorithm(const std::string& algorithmName) {
		assert(algorithmFactories.size() - 1 == algorithmNames.size());
		algorithmNames.push_back(algorithmName);
	}
public:
	friend class AlgorithmRegistration;
	enum { ALGORITHM_REGISTERED_SUCCESSFULY = 0, FILE_CANNOT_BE_LOADED = -1, NO_ALGORITHM_REGISTERED = -2 };
	int loadAlgorithm(const std::string& path, const std::string& so_file_name_without_so_suffix);
	vector<unique_ptr<AbstractAlgorithm>> getAlgorithms()const {
		vector<unique_ptr<AbstractAlgorithm>> algorithms;
		for (auto algorithmFactoryFunc : algorithmFactories) {
			algorithms.push_back(algorithmFactoryFunc());
		}
		return algorithms;
	}
	const list<std::string>& getAlgorithmNames()const {
		return algorithmNames;
	}
	size_t size()const {
		return algorithmFactories.size();
	}
	static AlgorithmRegistrar& getInstance() {
		return instance;
	}
private:
	static AlgorithmRegistrar instance;
};
