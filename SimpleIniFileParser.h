#ifndef SIMPLEINIFILEPARSER_H_
#define SIMPLEINIFILEPARSER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
using namespace std;



//string configPath = "config.ini";

class SimpleIniFileParser
{
public:

	SimpleIniFileParser();
	SimpleIniFileParser(const string& iniPath);
	bool loadFromFile(const string& iniPath);
	string toString();
	map<string, int> getMap();
	operator bool() const {
		return successOpenFile;
	}

private:
	map<string, int> parameters;
	bool successOpenFile;

	static vector<string> split(const string &s, char delim) {
		vector<string> elems;
		stringstream ss(s);
		string item;
		while (getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	static string trim(string& str)
	{
		str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
		str.erase(str.find_last_not_of(' ') + 1);         //surfixing spaces
		return str;
	}

	void processLine(const string& line)
	{
		vector<string> tokens = split(line, '=');
		if (tokens.size() != 2)
		{
			return;
		}
		this->parameters[trim(tokens[0])] = stoi(tokens[1]);
	}

};



#endif /*SIMPLEINIFILEPARSER_H_*/
