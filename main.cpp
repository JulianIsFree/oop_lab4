#include <iostream>
#include "CSVParser.h"
#include "Tuple.h"

int main()
{
	using namespace std;
	using namespace labCSVParser;
	using namespace labTuple;
	
	CSVParser<int, string, string> parser("input.txt");
	for (auto it = parser.begin(); it != parser.end(); ++it)
		cout << *it << endl;
	//std::tuple<int, int, string> t;
	//changeTuple(t);

	return 0;
}
