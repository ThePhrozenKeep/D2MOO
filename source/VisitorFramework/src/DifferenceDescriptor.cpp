#include "DifferenceDescriptor.h"

#include <ostream>
#include <vector>

#include "DifferenceType.h"


std::ostream& operator<<(std::ostream& os, const DifferenceDescriptor& descriptor)
{
	os << " {";

	switch (descriptor.type)
	{
	case DifferenceType::Field:
		os << "Field";
		break;

	case DifferenceType::Pointer:
		os << "Pointer";
		break;
	}

	os << ", " << descriptor.field_name << ", " << descriptor.object_value << ", " << descriptor.other_value << " }";

	return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<DifferenceDescriptor>& differences)
{
	os << "\n  {\n";
	for (const auto& d : differences)
	{
		os << "  " << d << '\n';
	}
	os << "  }\n";
	return os;
}
