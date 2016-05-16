#include "FuncForMain.h"
#include <memory>
#include <cstdio>
#define HOUSE_COMMAND "-house_path"
#define ALGO_COMMAND "-algorithm_path"
#define CONFIG_PATH "-config"
#define SCORE_PATH "-score_formula"
#define LEFTB "["
#define RIGHTB "]"



bool hasEnding(string const &fullString, string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}

bool IsPrefix(string const& fullstring, string const& prefix)
{
	return equal(
		fullstring.begin(),
		fullstring.begin() + min(fullstring.size(), prefix.size()),
		prefix.begin());
}

string ConvertHouseStatetoString(houseState state) {
	string msg;
	if (state == NO_DOCKING)
		msg = "missing docking station (no D in house)";
	if (state == MORE_THAN_ONE_DOCKING)
		msg = "too many docking stations (more than one D in house)";
	return msg;
}

void createVcetorFromMapValues(vector <string>& vec, map<string, string> m, map<string, string> errorMap) {
	for (auto elem : m)
		if (errorMap.find(elem.first) == errorMap.end())
			vec.push_back(elem.second);
}

void PrintErrors(map<string, string> errorMap, map<string, string> nameMap, string typefile) {
	for (auto it = errorMap.cbegin(); it != errorMap.cend(); ++it) {
		cout << nameMap[it->first] << "." << typefile << ": " << it->second << endl;
	}
}
inline bool isInteger(const std::string & s)
{
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

	char * p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
}
bool badValue(const string& str) {
	if (!isInteger(str))
		return true;
	return stoi(str) <= 0;
}

void checkVectorByMap(map<string, string> map1, vector<string> vector1, vector<bool>* boolV) {
	for (size_t i = 0; i < vector1.size(); i++) {
		(*boolV)[i] = !(map1.find(vector1[i]) == map1.end());
	}
}
void checkBadValues(map<string,string> map1,vector<string> vec1, vector<bool>* boolV) {
	for (unsigned int i = 0; i < vec1.size(); i++) {
		(*boolV)[i] = (!badValue(map1[vec1[i]]));
	}
}
void  printConfiError(int numberOfMissing, vector<bool> checkAllConfi, vector<string> confiVector,string msg) {
	int cnt = 0;
	cout << msg << numberOfMissing << " parameter(s): ";
	for (size_t i = 0; i < checkAllConfi.size(); i++) {
		if (!checkAllConfi[i]) {
			cnt++;
			(cnt == numberOfMissing) ? cout << confiVector[i] << "."
				: cout << confiVector[i] << ", ";
		}
	}
	cout << endl;

}
bool checkConfig(string fileName, map<string, string>* config, string config_path) {
	SimpleIniFileParser iniParser(fileName);
	if (!iniParser) {
		cout << "config.ini exists in '" << config_path << "' but cannot be opened" << endl;
		return false;
	}
	*config = iniParser.getMap();
	vector<string> confiVector = { "MaxStepsAfterWinner",
		"BatteryCapacity", "BatteryConsumptionRate",
		"BatteryRechargeRate" };
	vector<bool> checkAllConfi(confiVector.size());
	checkVectorByMap(*config, confiVector, &checkAllConfi);
	unsigned int numberOfMissingConfi;
	numberOfMissingConfi = std::count(checkAllConfi.begin(), checkAllConfi.end(), false);
	if (numberOfMissingConfi > 0) {
		printConfiError(numberOfMissingConfi, checkAllConfi, confiVector, "config.ini missing ");
		return false;
	}
	//after knowing that no missing confi,check bad value with the same vector
	checkBadValues(*config, confiVector, &checkAllConfi);
	unsigned int numberOfBadConfi;
	numberOfBadConfi = std::count(checkAllConfi.begin(), checkAllConfi.end(), false);
	if (numberOfBadConfi > 0) {
		printConfiError(numberOfBadConfi, checkAllConfi, confiVector, "config.ini having bad values for ");
		return false;
	}
	return true;
}

int checkHouses(map<string, string> houseNames, vector<House*>* houses, map<string, string>* errorHouse) {
	string stringErrorMSGHouse;
	for (auto houseName : houseNames) {
		House *house = new House();
		house->fillHouseData(houseName.first, stringErrorMSGHouse);
		if (stringErrorMSGHouse == "") {
			houseState state;
			Point point(-1, -1);
			if ((state = house->checkIfHouseLegal(point)) != GOOD_HOUSE)
				(*errorHouse)[houseName.first] = ConvertHouseStatetoString(state);
			else
				(*houses).push_back(house);
		}
		else
			(*errorHouse)[houseName.first] = stringErrorMSGHouse;
	}
	return houses->size();
}
int findIndexOfElem(vector<string> Names, string name) {
	auto it = std::find(Names.begin(), Names.end(), name);
	if (it == Names.end())
	{
		return -1;
	}
	else
	{
		return  std::distance(Names.begin(), it);
	}
}
void checkArguments(int argc, char** argv, string& config_path, string& algo_path, string& house_path,string& score_path) {
	vector<string> commandLineOptions(argc - 1);
	for (int i = 1; i < argc; i++)
		commandLineOptions[i - 1] = argv[i];
	int pos_score = findIndexOfElem(commandLineOptions, "-score_formula");
	cout << "score is " << pos_score << endl;
	if (pos_score < static_cast<int>(commandLineOptions.size()) - 1 && pos_score != -1)
		score_path = commandLineOptions[pos_score + 1];
	else
		score_path = "";
	int pos_algo = findIndexOfElem(commandLineOptions, ALGO_COMMAND);
	if (pos_algo < static_cast<int>(commandLineOptions.size()) - 1 && pos_algo != -1)
		algo_path = commandLineOptions[pos_algo + 1];
	else
		algo_path = fs::current_path().string();
	int pos_house = findIndexOfElem(commandLineOptions, HOUSE_COMMAND);
	if (pos_house < static_cast<int>(commandLineOptions.size()) - 1 && pos_house != -1)
		house_path = commandLineOptions[pos_house + 1];
	else
		house_path = fs::current_path().string();
	int pos_config = findIndexOfElem(commandLineOptions, CONFIG_PATH);
	if (pos_config < static_cast<int>(commandLineOptions.size()) - 1 && pos_config != -1)
		config_path = commandLineOptions[pos_config + 1];
	else
		config_path = fs::current_path().string();
}

bool updateFilesFromDirectory(map<string, string>& fileNameMap, string typeFiles, string& directory) {

	fs::path p(directory);
	fs::path fullpath_dir(fs::complete(p));
	directory = fullpath_dir.string();
	fs::directory_iterator end_iter;
	if (fs::exists(fullpath_dir) && fs::is_directory(fullpath_dir)) {
		for (fs::directory_iterator dir_iter(fullpath_dir); dir_iter != end_iter; dir_iter++) {
			if (fs::is_regular_file(dir_iter->status())) {
				fs::path path = *dir_iter;
				if (hasEnding(path.string(), typeFiles))
					fileNameMap[path.string()] = path.stem().string();
			}
		}
	}
	return fileNameMap.size() > 0;
}

void Usage(string house_path, string config_path, string algo_path,bool house_exist,bool config_exist,bool algo_exist) {
	cout << "Usage: simulator " << LEFTB << CONFIG_PATH << " " << config_path << RIGHTB << " " << LEFTB << HOUSE_COMMAND << " "
		<< house_path << RIGHTB << " " << LEFTB << ALGO_COMMAND << " " << algo_path << RIGHTB << endl;
	if (!house_exist)
		cout << "cannot find house files in '" << house_path << "'" << endl;
	if (!config_exist)
		cout << "cannot find config.ini files in '" << config_path << "'" << endl;
	if (!algo_exist)
		cout << "cannot find algorithm files in '" << algo_path << "'" << endl;


}
void convertToMapInt(map <string, string>& stringMap, map<string, int>* intMap) {
	for (auto&  elem : stringMap)
		(*intMap)[elem.first] = stoi(elem.second);
}