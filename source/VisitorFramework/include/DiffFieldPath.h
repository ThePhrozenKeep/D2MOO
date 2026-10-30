#pragma once


#include <iterator>
#include <memory>
#include <string>
#include <utility>
#include <vector>


// The name of a nested field, e.g. "a->b->c[2]". Nodes share their parents, so comparing a long pointer chain does
// not copy an ever-growing name at every level. Turned into a string only when a difference is reported.
struct DiffFieldPath
{
	std::shared_ptr<DiffFieldPath> parent;
	std::string name;

	DiffFieldPath(std::shared_ptr<DiffFieldPath> parent, std::string name) : parent(std::move(parent)), name(std::move(name)) {}

	DiffFieldPath(const DiffFieldPath&) = delete;
	DiffFieldPath& operator=(const DiffFieldPath&) = delete;

	// Releases the parents iteratively, as releasing a long chain recursively could overflow the call stack
	~DiffFieldPath()
	{
		auto ancestor = std::move(parent);

		while (ancestor && ancestor.use_count() == 1)
		{
			ancestor = std::move(ancestor->parent);
		}
	}

	std::string ToString() const
	{
		std::vector<const DiffFieldPath*> nodes;

		for (auto* node = this; node != nullptr; node = node->parent.get())
		{
			nodes.push_back(node);
		}

		std::string result;

		for (auto it = nodes.rbegin(); it != nodes.rend(); ++it)
		{
			if (!result.empty())
			{
				result += "->";
			}

			result += (*it)->name;
		}

		return result;
	}
};
