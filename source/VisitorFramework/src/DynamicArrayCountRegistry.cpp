#include "DynamicArrayCountRegistry.h"


#include <cstdint>
#include <stdexcept>
#include <string_view>
#include <utility>


static constexpr uint64_t fnv1a(std::string_view s) {
	uint64_t hash = 14695981039346656037ull;

	for (unsigned char c : s) {
		hash ^= c;
		hash *= 1099511628211ull;
	}

	return hash;
}


DynamicArrayCountRegistry& DynamicArrayCountRegistry::Instance()
{
	static DynamicArrayCountRegistry registry;
	return registry;
}

void DynamicArrayCountRegistry::Register(std::string_view key, size_t count)
{
	Register(key, CountProvider([count]() { return count; }));
}

void DynamicArrayCountRegistry::Register(std::string_view key, CountProvider provider)
{
	if (!provider)
	{
		throw std::invalid_argument("Dynamic array count provider must not be empty.");
	}

	const auto hash = fnv1a(key);

	const auto it = m_entries.find(hash);

	if (it != m_entries.end() && it->second.key != key)
	{
		throw std::runtime_error("Dynamic array count key hash collision: '" + std::string(key) + "' and '" + it->second.key + "'.");
	}

	m_entries[hash] = Entry{ std::string(key), std::move(provider) };
}

void DynamicArrayCountRegistry::Unregister(std::string_view key)
{
	m_entries.erase(fnv1a(key));
}

bool DynamicArrayCountRegistry::Contains(std::string_view key) const
{
	return m_entries.find(fnv1a(key)) != m_entries.end();
}

size_t DynamicArrayCountRegistry::GetCount(std::string_view key) const
{
	const auto it = m_entries.find(fnv1a(key));

	if (it == m_entries.end())
	{
		throw std::runtime_error("No dynamic array count registered for key '" + std::string(key) + "'.");
	}

	return it->second.provider();
}
