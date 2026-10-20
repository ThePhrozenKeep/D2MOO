#pragma once


#include <cstdarg>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <utility>

#include <Windows.h>


static_assert(sizeof(void*) == 4, "InlineHook only supports 32-bit x86 (E9 rel32 jumps)");

// Overwrites the first 5 bytes of a function with a relative jump to a replacement function.
// The original bytes are restored when the hook is destroyed.
// Caller and replacement must share the same signature and calling convention.
class InlineHook
{
public:
	InlineHook(void* target, const void* replacement)
		: m_target(static_cast<uint8_t*>(target))
	{
		if (m_target == nullptr || replacement == nullptr)
		{
			throw std::runtime_error("InlineHook: target or replacement is null");
		}

		std::memcpy(m_original_bytes, m_target, sizeof(m_original_bytes));

		uint8_t patch[5]{};
		patch[0] = 0xE9;
		const auto relative_offset = static_cast<int32_t>(reinterpret_cast<uintptr_t>(replacement) - (reinterpret_cast<uintptr_t>(m_target) + sizeof(patch)));
		std::memcpy(&patch[1], &relative_offset, sizeof(relative_offset));

		write(patch);
	}

	InlineHook(const InlineHook&) = delete;
	InlineHook& operator=(const InlineHook&) = delete;

	InlineHook(InlineHook&& other) noexcept
		: m_target(std::exchange(other.m_target, nullptr))
	{
		std::memcpy(m_original_bytes, other.m_original_bytes, sizeof(m_original_bytes));
	}

	InlineHook& operator=(InlineHook&&) = delete;

	~InlineHook()
	{
		if (m_target)
		{
			write(m_original_bytes);
		}
	}

private:
	void write(const uint8_t(&bytes)[5])
	{
		DWORD old_protect = 0;
		if (!VirtualProtect(m_target, sizeof(bytes), PAGE_EXECUTE_READWRITE, &old_protect))
		{
			throw std::runtime_error("InlineHook: VirtualProtect failed");
		}

		std::memcpy(m_target, bytes, sizeof(bytes));

		VirtualProtect(m_target, sizeof(bytes), old_protect, &old_protect);
		FlushInstructionCache(GetCurrentProcess(), m_target, sizeof(bytes));
	}

	uint8_t* m_target;
	uint8_t m_original_bytes[5];
	uint8_t pad[3]{};
};

// Returns the address that `module` will call for the function imported by ordinal from `dll_name`,
// as resolved by the loader in its import address table. Returns nullptr if it is not imported.
inline void* find_imported_function_by_ordinal(HMODULE module, const char* dll_name, WORD ordinal)
{
	const auto base = reinterpret_cast<uint8_t*>(module);
	const auto dos_header = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
	const auto nt_headers = reinterpret_cast<const IMAGE_NT_HEADERS*>(base + dos_header->e_lfanew);
	const auto& import_directory = nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

	if (import_directory.VirtualAddress == 0)
	{
		return nullptr;
	}

	for (auto descriptor = reinterpret_cast<const IMAGE_IMPORT_DESCRIPTOR*>(base + import_directory.VirtualAddress); descriptor->Name; ++descriptor)
	{
		if (_stricmp(reinterpret_cast<const char*>(base + descriptor->Name), dll_name) != 0)
		{
			continue;
		}

		if (descriptor->OriginalFirstThunk == 0)
		{
			// Without a lookup table, the ordinals were overwritten at load time. Ask the loaded DLL instead.
			const auto imported_module = GetModuleHandleA(dll_name);
			return imported_module ? reinterpret_cast<void*>(GetProcAddress(imported_module, MAKEINTRESOURCEA(ordinal))) : nullptr;
		}

		auto lookup_entry = reinterpret_cast<const IMAGE_THUNK_DATA*>(base + descriptor->OriginalFirstThunk);
		auto iat_entry = reinterpret_cast<const IMAGE_THUNK_DATA*>(base + descriptor->FirstThunk);
		for (; lookup_entry->u1.AddressOfData; ++lookup_entry, ++iat_entry)
		{
			if (IMAGE_SNAP_BY_ORDINAL(lookup_entry->u1.Ordinal) && IMAGE_ORDINAL(lookup_entry->u1.Ordinal) == ordinal)
			{
				return reinterpret_cast<void*>(iat_entry->u1.Function);
			}
		}
	}

	return nullptr;
}
