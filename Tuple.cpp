#include "Tuple.h"

std::ostream& labTuple::operator<<(std::ostream& os, const std::tuple<>& t)
{
	return os;
}

bool labTuple::operator==(const std::tuple<>& t1, const std::tuple<>& t2)
{
	return true;
}

bool labTuple::operator!=(const std::tuple<>& t1, const std::tuple<>& t2)
{
	return false;
}