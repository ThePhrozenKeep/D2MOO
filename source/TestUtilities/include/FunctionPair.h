#pragma once


#include <cstdarg>
#include <utility>


template<typename Function>
constexpr auto make_function_pair(Function& function, uintptr_t original_address)
{
	using FunctionPtr = decltype(&function);

	return std::pair{ &function, reinterpret_cast<FunctionPtr>(original_address) };
}
