#include <iostream>
#include "CSVParser.h"
#include "Tuple.h"

int main()
{
	using namespace std;
	using namespace labCSVParser;
	using namespace labTuple;
	
	CSVParser<int, int> parser("input.txt");
	for (auto it = parser.begin(); it != parser.end(); ++it)
		cout << *it << endl;

	return 0;
}
