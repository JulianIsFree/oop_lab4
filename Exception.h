#pragma once
#include <exception>
#include <string>

class Exception : public std::exception
{
public:
	Exception(const std::string& message) : std::exception(message.c_str()) {};
};

class CSVParsingException : public Exception
{
	std::string getCoords(size_t r, size_t c)
	{
		return "(" + std::to_string(r) + "," + std::to_string(c) + ")";
	}
public:
	CSVParsingException(const std::string& what, size_t r, size_t c) : Exception(what + " at " + getCoords(r, c)) {};
};