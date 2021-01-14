#ifndef CSVPARSER_H
#define CSVPARSER_H
#include <fstream>
#include <tuple>
#include "Exception.h"
#include "Tuple.h"

namespace labCSVParser
{	
	template<class... Types>
	class CSVParser
	{
		std::ifstream fin;

		std::string delims;
		size_t column;
		size_t row;

		class iterator
		{
			friend class CSVParser;
			CSVParser &pars;
			std::tuple<Types...> t;
			bool end;
		public:

			iterator(CSVParser &pars, const std::tuple<Types...>& t, bool end=false) : pars(pars), end(end), t(t) {};
			const std::tuple<Types...>& operator*() const 
			{ 
				return t; 
			};
			
			bool operator==(const iterator& it) const 
			{ 
				if (end && it.end) 
					return true; 

				return it.t == t;
			}

			bool operator!=(const iterator& it) const
			{
				return !(*this == it);
			}

			iterator& operator++()
			{
				end = !pars.getWord(t, t);
				return *this;
			}

			iterator operator++(int)
			{
				iterator it(pars, t, end);
				this->operator++();
				return it;
			}
		};
		friend class iterator;

		template<size_t N, class First, class... Rest>
		void readItem(std::tuple<First, Rest...>& tuple)
		{
			try 
			{
				column = N;
				First temp;
				fin >> temp;
				std::get<N>(tuple) = temp;
			}
			catch (std::exception &e)
			{
				throw CSVParsingException(e.what(), row, column);
			}
		}

		bool getWord(std::tuple<>& tuple, std::tuple<>& t)
		{
			return false;
		}

		template<size_t N, class ...Types, class First>
		bool getWord(std::tuple<Types...> &tuple, std::tuple<First>& t)
		{
			if (fin.eof())
				return false;

			readItem<N>(tuple);
			row++;
			return true;
		};

		template<size_t N=0, class...Types, class First, class... Rest>
		bool getWord(std::tuple<Types...>& tuple, std::tuple<First, Rest...>& t)
		{
			if (fin.eof())
				return false;
			readItem<N>(tuple);
			return getWord<N + 1>(tuple, t._Get_rest());
		};
	public:
		CSVParser<Types...>(const std::string &fileName) : fin(fileName) {};

		std::tuple<Types...> getNext()
		{
			std::tuple<Types...> t;
			getWord(t, t);
			return t;
		}

		bool hasNext()
		{
			std::tuple<Types...> t;
			auto pos = fin.tellg();
			bool result = getWord(t, t);
			fin.seekg(pos);

			return result;
		}

		iterator begin()
		{
			row = 0;
			fin.seekg(0);
			std::tuple<Types...> t;
			bool res = !getWord(t, t);
			iterator it(*this, t, res);

			return it;
		}

		iterator end()
		{
			return iterator(*this, std::tuple<Types...>(), true);
		}
	};
}

#endif