#pragma once


#include <ostream>
#include <set>
#include <string>
#include <sstream>
#include <vector>

#include "DifferenceDescriptor.h"


struct DiffContext
{
	const char* title;
	std::vector<DifferenceDescriptor> differences;
	std::set<void*> visited_pointers;

	explicit DiffContext(const char* title) : title(title), differences() {}

	std::string DifferenceDescription()
	{
		std::stringstream stream;

		if (differences.empty())
		{
			return "";
		}

		if (title != nullptr)
		{
			stream << "\n\n" << title << '\n';
		}

		stream << "Found \x1b[33m" << differences.size() << "\x1b[0m difference(s):\n\n";

		for (const auto& difference : differences)
		{
			stream << "Field: \x1b[33m" << difference.field_name << "\x1b[0m\n";
			stream << "  MOO      value: \x1b[31m" << difference.object_value << "\x1b[0m\n";
			stream << "  Original value: \x1b[31m" << difference.other_value << "\x1b[0m\n";
			stream << '\n';
		}

		stream << "---------------------------" << std::endl;

		return stream.str();
	}
};
