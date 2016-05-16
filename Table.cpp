#include "Table.h"
#include <map>
#include <string>


void Table::printEmptyLine()
{
	string line(placeForLine(),'-');
	cout << line << endl;
}

void Table::printHouseNamesLine()
{
	string line("");
	line += SEPRATE_CHAR + string(PLACE_FOR_ALGO, ' ') + SEPRATE_CHAR;
	for (auto name : houseNames) {
		string check = name.substr(0,PLACE_FOR_HOUSE - 1);
		line += (name.size() > PLACE_FOR_HOUSE - 1) ? name.substr(0, PLACE_FOR_HOUSE - 1) + " " + SEPRATE_CHAR :
			name + string(PLACE_FOR_HOUSE - name.size(), ' ') + SEPRATE_CHAR;
	}
	line += "AVG       |";
	cout << line << endl;
}

bool Table::printTable(scoreFunc scorefunc)
{
	bool flag = true;
	printEmptyLine();
	printHouseNamesLine();
	multimap <double, string,greater<double>> sortedTable;
	for (int i = 0; i < n; i++) {
		string line("");
		line += (algoNames[i].size() >= PLACE_FOR_ALGO) ? SEPRATE_CHAR + algoNames[i].substr(0, PLACE_FOR_ALGO)
			: SEPRATE_CHAR + algoNames[i] + string(PLACE_FOR_ALGO - algoNames[i].size(), ' ');
		int sum = 0;
		for (int j = 0; j < m; j++) {
			Score score = robots[j*n + i]->getScore();
			map<string, int> mapForScore;
			score.createMapForScore(mapForScore);
			//int result = score.calcResult();
			int result = scorefunc ? scorefunc(mapForScore) : score.calcResult();
			if (result == -1)
				flag = false;
			sum += result;
			string res = std::to_string(result);
			int placeForSpaces = PLACE_FOR_HOUSE - res.size();
			line += SEPRATE_CHAR + string(placeForSpaces, ' ') +res;
		}
		double avg=sum/(double)m;
		string res = std::to_string(avg);
		int placeForSpace = PLACE_FOR_HOUSE - res.size() + 4; //because we want 2 digit and to_string is 6 digit
		line += SEPRATE_CHAR + string(placeForSpace, ' ') + res.substr(0,res.size() - 4) + SEPRATE_CHAR;
		sortedTable.insert(std::pair<double, string>(avg, line));
		//printEmptyLine();
		//cout << line << endl;
	}
	printEmptyLine();
	for (auto elem : sortedTable) {
		cout << elem.second << endl;
		printEmptyLine();
	}
	//printEmptyLine();
	return flag;
}

Table::~Table()
{
}
