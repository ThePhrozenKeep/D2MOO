#pragma once


#include <cstdarg>
#include <filesystem>
#include <stdexcept>
#include <string>

#include <Windows.h>


static auto load_base_address(const std::string& module_name, const std::filesystem::path& working_directory = std::filesystem::current_path()) -> std::uintptr_t
{
	const auto module_handle = LoadLibraryA((working_directory / module_name).string().c_str());
	if (!module_handle)
	{
		throw std::runtime_error("Cannot get module handle");
	}

	return reinterpret_cast<std::uintptr_t>(module_handle);
}
