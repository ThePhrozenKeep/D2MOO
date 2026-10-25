#pragma once


#include <cmath>
#include <cstddef>
#include <cstring>
#include <sstream>
#include <string>


template <typename T>
struct TestComparator {
	static bool compare(T& a, T& b, const char* context_title, std::string& diff_description)
	{
		const auto equal = a == b;
		if (equal)
		{
			return true;
		}

		std::stringstream stream;
		stream << "\n\n" << context_title << '\n';
		stream << a << " (MOO) is NOT equal to " << b << " (Original)\n";
		diff_description = stream.str();
		return false;
	}
};

template <std::size_t N>
struct TestComparator<char[N]>
{
	static bool compare(char(&a)[N], char(&b)[N], const char* context_title, std::string& diff_description)
	{
		const auto equal = std::strcmp(a, b) == 0;
		if (equal)
		{
			return true;
		}

		std::stringstream stream;
		stream << "\n\n" << context_title << '\n';
		stream << a << " (MOO) is NOT equal to " << b << " (Original)\n";
		diff_description = stream.str();
		return false;
	}
};

template<>
struct TestComparator<float>
{
	static bool compare(float a, float b, const char* context_title, std::string& diff_description)
	{
		const auto equal = std::abs(a - b) < 0.0001;
		if (equal)
		{
			return true;
		}

		std::stringstream stream;
		stream << "\n\n" << context_title << '\n';
		stream << a << " (MOO) is NOT equal to " << b << " (Original)\n";
		diff_description = stream.str();
		return false;
	}
};

template<>
struct TestComparator<double>
{
	static bool compare(double a, double b, const char* context_title, std::string& diff_description)
	{
		const auto equal = std::abs(a - b) < 0.0001;
		if (equal)
		{
			return true;
		}

		std::stringstream stream;
		stream << "\n\n" << context_title << '\n';
		stream << a << " (MOO) is NOT equal to " << b << " (Original)\n";
		diff_description = stream.str();
		return false;
	}
};
