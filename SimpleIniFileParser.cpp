#include "SimpleIniFileParser.h"

SimpleIniFileParser::SimpleIniFileParser() { }
SimpleIniFileParser:: SimpleIniFileParser(const string& iniPath)
{
	successOpenFile = this->loadFromFile(iniPath);
		
}

bool SimpleIniFileParser::loadFromFile(const string& iniPath)
	{
		this->parameters.clear();
		ifstream fin(iniPath.c_str());
		if (!fin) {
			return false;
		}
		string line;
		while (getline(fin, line))
		{
			this->processLine(line);
		}
		return true;
	}

string SimpleIniFileParser::toString()
	{
		string out;
		for (map<string, string>::const_iterator itr = this->parameters.begin();
			itr != this->parameters.end();
			++itr)
		{
			if (itr != this->parameters.begin())
			{
				out += "\n";
			}
			out += "parameters[" + itr->first + "] = " +itr->second;
		}
		return out;
	}


map<string, string> SimpleIniFileParser::getMap(){
	return parameters;
}


