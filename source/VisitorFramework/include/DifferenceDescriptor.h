#pragma once


#include <ostream>
#include <string>
#include <vector>

#include "DifferenceType.h"


struct DifferenceDescriptor
{
	DifferenceType type;
	std::string field_name;
	std::string object_value;
	std::string other_value;

	bool operator==(const DifferenceDescriptor& rhs) const
	{
		return type == rhs.type &&
			field_name == rhs.field_name &&
			object_value == rhs.object_value &&
			other_value == rhs.other_value;
	}
};


std::ostream& operator<<(std::ostream& os, const DifferenceDescriptor& descriptor);

std::ostream& operator<<(std::ostream& os, const std::vector<DifferenceDescriptor>& differences);
