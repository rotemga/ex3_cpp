#ifndef FUNCFORMAIN_H_
#define FUNCFORMAIN_H_

#include <iostream>
#include <fstream>
#include <vector>
#include "House.h"
#include <string>
#include "Simulator.h"
#include "SimpleIniFileParser.h"
#include <thread>

#include "boost/filesystem.hpp"   // includes all needed Boost.Filesystem declarations

using namespace std;
namespace fs = boost::filesystem;
void convertToMapInt(map <string, string>& stringMap, map<string, int>* intMap);
bool updateFilesFromDirectory(map<string, string>& fileMap, string typeFiles, string& directory);
bool hasEnding(string const &fullString, string const &ending);
bool IsPrefix(string const& fullstring, string const& prefix);
string ConvertHouseStatetoString(houseState state);
void createVcetorFromMapValues(vector <string>&, map<string, string>, map<string, string>);
void PrintErrors(map<string, string> errorMap, map<string, string> mapName, string type);
void checkVectorByMap(map<string, string> map1, vector<string> vector1, vector<bool>* boolV);
void  printConfiError(int numberOfMissing, vector<bool> checkAllConfi, vector<string> confiVector,string msg);
bool checkConfig(string fileName, map<string, string>* config, string config_path);
int checkHouses(map<string, string> houseNames, vector<House*>* houses, map<string, string>* errorHouse);
void checkArguments(int argc, char** argv, string& config_path, string& algo_path, string& house_path,string& score_path);
void updateCurrDir(vector<fs::path>* fileName_currDir);
void Usage(string house_path, string config_path, string algo_path,bool house_exist,bool config_exist,bool algo_exist);
#endif /* FUNCFORMAIN_H_ */