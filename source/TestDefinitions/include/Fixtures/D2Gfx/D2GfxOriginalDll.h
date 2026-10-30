#pragma once


#include <cstdint>
#include <cstring>
#include <filesystem>
#include <sstream>
#include <stdexcept>

#include <Windows.h>


// Base address of the original D2Gfx.dll (1.10f). Addresses in comments and global names are relative to it.
constexpr uintptr_t D2GFX_ORIGINAL_IMAGE_BASE = 0x6FA70000;


inline uintptr_t d2gfx_original_dll_base()
{
	static const auto dll_base = []()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto module = LoadLibraryA((working_directory / "D2Gfx.dll").string().c_str());
		if (module == nullptr)
		{
			throw std::runtime_error("Cannot load original D2Gfx.dll");
		}

		return reinterpret_cast<uintptr_t>(module);
	}();

	return dll_base;
}

// Global of the original dll at the given RVA
template<typename T>
T& d2gfx_original_global(uintptr_t rva)
{
	return *reinterpret_cast<T*>(d2gfx_original_dll_base() + rva);
}

// Some globals have no known address, but are read or written by the first instruction of a function.
// The address is taken from the operand of that instruction, so it also works if the dll was relocated.
inline uintptr_t d2gfx_read_global_address_from_function(uintptr_t function_rva)
{
	const auto dll_base = d2gfx_original_dll_base();
	const auto* code = reinterpret_cast<const uint8_t*>(dll_base + function_rva);

	uint32_t address = 0;
	if (code[0] == 0xA1 || code[0] == 0xA3)
	{
		// mov eax, [imm32] / mov [imm32], eax
		std::memcpy(&address, &code[1], sizeof(address));
	}
	else if ((code[0] == 0x8B || code[0] == 0x89) && (code[1] & 0xC7) == 0x05)
	{
		// mov r32, [imm32] / mov [imm32], r32
		std::memcpy(&address, &code[2], sizeof(address));
	}
	else
	{
		std::stringstream message;
		message << "Unexpected instruction at D2Gfx.0x" << std::hex << (D2GFX_ORIGINAL_IMAGE_BASE + function_rva) << ", cannot find the address of a global";
		throw std::runtime_error(message.str());
	}

	const auto* dos_header = reinterpret_cast<const IMAGE_DOS_HEADER*>(dll_base);
	const auto* nt_headers = reinterpret_cast<const IMAGE_NT_HEADERS*>(dll_base + dos_header->e_lfanew);
	if (address < dll_base || address >= dll_base + nt_headers->OptionalHeader.SizeOfImage)
	{
		std::stringstream message;
		message << "Address read at D2Gfx.0x" << std::hex << (D2GFX_ORIGINAL_IMAGE_BASE + function_rva) << " is not inside D2Gfx.dll";
		throw std::runtime_error(message.str());
	}

	return address;
}

// Global of the original dll accessed by the first instruction of the function at the given RVA, minus the offset of the accessed member
template<typename T>
T& d2gfx_original_global_from_function(uintptr_t function_rva, uintptr_t member_offset = 0)
{
	return *reinterpret_cast<T*>(d2gfx_read_global_address_from_function(function_rva) - member_offset);
}
