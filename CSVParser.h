#ifndef CSVPARSER_H
#define CSVPARSER_H
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <tuple>
#include <locale>

#include "Exception.h"
#include "Tuple.h"

namespace labCSVParser
{
	struct DelimsLocale : std::ctype<char>
	{
		DelimsLocale(std::string const &delims) : std::ctype<char>(get_table(delims)) {}

		static std::ctype_base::mask const* get_table(std::string const &delims)
		{
			static std::vector<std::ctype_base::mask> delimiters(table_size, std::ctype_base::mask());
			for (unsigned char ch : delims)
				delimiters[ch] = std::ctype_base::space;

			return &delimiters[0];
		}
	};

	template<class... Types>
	class CSVParser
	{
		std::ifstream fin;
		std::istringstream sin;

		std::string delims;
		size_t column;
		size_t row;

		class iterator
		{
			friend class CSVParser;
			CSVParser &pars;
			std::tuple<Types...> t;
			bool end;

			iterator(CSVParser &pars, const std::tuple<Types...>& t, bool end=false) : pars(pars), end(end), t(t) {};
		public:
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
				end = !pars.hasNext();
				if (!end)
					t = pars.getNext();
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
				sin >> temp;
				std::get<0>(tuple) = First(temp);
			}
			catch (std::exception &e)
			{
				throw CSVParsingException(e.what(), row, column);
			}
		}

		std::string removeShieldedData(const std::string& line)
		{
			return line;
		}

		void prepareLine()
		{
			std::string line;
			std::getline(fin, line);
			line = removeShieldedData(line);
			sin = std::istringstream(line);
			sin.imbue(std::locale(std::locale(), new DelimsLocale(delims)));
		}

		template<size_t N>
		bool getWord(std::tuple<>& t)
		{
			return false;
		}

		template<size_t N, class First>
		bool getWord(std::tuple<First>& t)
		{
			if (sin.eof())
				return false;

			readItem<N>(t);
			return true;
		}

		template<size_t N=0, class First, class... Rest>
		bool getWord(std::tuple<First, Rest...>& t)
		{
			if (sin.eof())
				return false;

			readItem<N>(t);
			return getWord<N+1>(t._Get_rest());
		}

	public:
		CSVParser<Types...>(const std::string &fileName, const std::string& delims = ",") : fin(fileName), delims(delims) {};

		std::tuple<Types...> getNext()
		{
			std::tuple<Types...> t;
			prepareLine();
 			getWord(t);
			return t;
		}

		bool hasNext()
		{
			std::tuple<Types...> t;
			auto pos = fin.tellg();
			prepareLine();
			bool result = getWord(t);
			fin.seekg(pos);

			return result;
		}

		iterator begin()
		{
			row = 0;
			fin.seekg(0);
			std::tuple<Types...> t;

			bool res = hasNext();
			if (res)
				t = getNext();

			iterator it(*this, t, !res);

			return it;
		}

		iterator end()
		{
			return iterator(*this, std::tuple<Types...>(), true);
		}
	};
}

#endif