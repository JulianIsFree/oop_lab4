#ifndef TUPLE_H
#define TUPLE_H
#include <iostream>
#include <initializer_list>
#include <tuple>

namespace labTuple
{
	std::ostream& operator<<(std::ostream& os, const std::tuple<>& t);

	template <class T>
		std::ostream& operator<<(std::ostream& os, const std::tuple<T>& t)
	{
		using namespace std;
		os << get<0>(t);
		return os;
	};

	template <class First, class... Rest>
		std::ostream& operator<<(std::ostream& os, const std::tuple<First, Rest...>& t)
	{
		using namespace std;
		os << get<0>(t) << ", ";

		return os << t._Get_rest();
	};

	// operator== for tuples
	bool operator==(const std::tuple<>& t1, const std::tuple<>& t2);

	template <class T>
		bool operator==(const std::tuple<T>& t1, const std::tuple<T>& t2)
	{
		return std::get<0>(t1) == std::get<0>(t2);
		};

	template <class First, class... Rest>
		bool operator==(const std::tuple<First, Rest...>& t1, const std::tuple<First, Rest...>& t2)
	{
		tuple<Rest...> tt1;
		tuple<Rest...> tt2;
		std::tie(std::ignore, tt1) = t1;
		std::tie(std::ignore, tt2) = t2;

		return (std::get<0>(t1) == std::get<0>(t2)) && operator==(tt1, tt2);
		};

	// operator!= for tuples
	bool operator!=(const std::tuple<>& t1, const std::tuple<>& t2);

	template <class T>
		bool operator!=(const std::tuple<T>& t1, const std::tuple<T>& t2)
	{
		return !operator==(t1, t2);
	};

	template <class First, class... Rest>
		bool operator!=(const std::tuple<First, Rest...>& t1, const std::tuple<First, Rest...>& t2)
	{
		return !operator==(t1, t2);
	};

	// logic for tuples with different templates
	template <class... Rest1, class... Rest2>
		constexpr bool operator==(const std::tuple<Rest1...>& t1, const std::tuple<Rest2...>& t2)
	{
		return false;
	};

	template <class... Rest1, class... Rest2>
		constexpr bool operator!=(const std::tuple<Rest1...>& t1, const std::tuple<Rest2...>& t2)
	{
		return !operator==(t1,t2);
	};
}
#endif