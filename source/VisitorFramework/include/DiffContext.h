#pragma once


#include <map>
#include <ostream>
#include <string>
#include <sstream>
#include <typeindex>
#include <utility>
#include <vector>

#include "DifferenceDescriptor.h"


struct DiffContext
{
	const char* title;
	std::vector<DifferenceDescriptor> differences;

	// Objects already compared, keyed by address and type (a struct and its first member share an address),
	// mapped to the object they were compared with. Tracked for both sides to detect differently shared objects.
	using VisitedKey = std::pair<const void*, std::type_index>;
	std::map<VisitedKey, const void*> visited_objects;
	std::map<VisitedKey, const void*> visited_other_objects;

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
