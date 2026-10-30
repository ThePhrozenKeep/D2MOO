#pragma once


#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>


// Stores element counts for dynamic arrays whose count is not a member of the visited struct
// (e.g. it is derived from global state). Counts are looked up by key when the array is visited.
class DynamicArrayCountRegistry
{
public:
	using CountProvider = std::function<size_t()>;

	static DynamicArrayCountRegistry& Instance();

	// Registers a fixed count. Registering an existing key replaces its count.
	void Register(std::string_view key, size_t count);

	// Registers a provider which is evaluated each time the count is requested.
	// Registering an existing key replaces its provider.
	void Register(std::string_view key, CountProvider provider);

	void Unregister(std::string_view key);

	bool Contains(std::string_view key) const;

	// Throws if no count has been registered for the key.
	size_t GetCount(std::string_view key) const;

private:
	struct Entry
	{
		std::string key;
		CountProvider provider;
	};

	std::unordered_map<uint64_t, Entry> m_entries;
};
