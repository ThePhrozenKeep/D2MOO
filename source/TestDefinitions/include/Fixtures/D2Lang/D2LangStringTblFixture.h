#pragma once


#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

#include <Windows.h>

#include <D2Lang.h>
#include <InlineHook.h>


// Replaces D2LANG_GetStringFromTblIndex (D2Lang.#10004) and D2LANG_GetTblIndex (D2Lang.#10013) for both the D2MOO implementation
// and the original D2Common.dll, so that no string table needs to be loaded.
// The fake GetStringFromTblIndex returns a string whose first character is the requested index, so comparing results compares the indices.
// The fake GetTblIndex returns an index computed from the reference string (0 for an empty string, like an unknown reference).
// Every call is recorded in `d2lang_string_tbl_calls` / `d2lang_tbl_index_calls`, which tests can clear and compare between both implementations.
struct D2LangStringTblMock
{
	static inline uint16_t fake_strings[0x10000][2]{};
	static inline std::vector<short> calls;
	static inline std::vector<std::string> tbl_index_calls;

	static const Unicode* __fastcall GetStringFromTblIndex(short nIndex)
	{
		calls.push_back(nIndex);

		return get_fake_string(nIndex);
	}

	static short __stdcall GetTblIndex(char* szReference, const Unicode** pUnicode)
	{
		tbl_index_calls.emplace_back(szReference ? szReference : "");

		short nIndex = 0;
		if (szReference && *szReference)
		{
			uint32_t nHash = 0;
			for (const char* pChar = szReference; *pChar; ++pChar)
			{
				nHash = nHash * 31 + static_cast<uint8_t>(*pChar);
			}
			nIndex = static_cast<short>(nHash % 0x7FFF + 1);
		}

		if (pUnicode)
		{
			*pUnicode = nIndex ? get_fake_string(nIndex) : nullptr;
		}

		return nIndex;
	}

private:
	static const Unicode* get_fake_string(short nIndex)
	{
		auto& fake_string = fake_strings[static_cast<uint16_t>(nIndex)];
		fake_string[0] = static_cast<uint16_t>(nIndex);
		fake_string[1] = 0;
		return reinterpret_cast<const Unicode*>(fake_string);
	}
};

template<class Fixture>
struct D2LangStringTblFixture : Fixture
{
	std::vector<short>& d2lang_string_tbl_calls = D2LangStringTblMock::calls;
	std::vector<std::string>& d2lang_tbl_index_calls = D2LangStringTblMock::tbl_index_calls;
	std::vector<InlineHook> d2lang_hooks;

	D2LangStringTblFixture()
	{
		constexpr WORD D2LANG_GETSTRINGFROMTBLINDEX_ORDINAL = 10004;
		constexpr WORD D2LANG_GETTBLINDEX_ORDINAL = 10013;

		const auto working_directory = std::filesystem::current_path();
		const auto original_d2common = LoadLibraryA((working_directory / "D2Common.dll").string().c_str());
		if (original_d2common == nullptr)
		{
			throw std::runtime_error("D2LangStringTblFixture: Cannot load original D2Common.dll");
		}

		// Function called by the original D2Common.dll (the original D2Lang.dll, or D2MOO's if it was already loaded)
		void* original_target = find_imported_function_by_ordinal(original_d2common, "D2Lang.dll", D2LANG_GETSTRINGFROMTBLINDEX_ORDINAL);
		// Function called by the D2MOO implementation linked into the test executable
		void* moo_target = reinterpret_cast<void*>(&D2LANG_GetStringFromTblIndex);

		if (original_target == nullptr)
		{
			throw std::runtime_error("D2LangStringTblFixture: Original D2Common.dll does not import D2Lang.#10004");
		}

		// Note: with identical COMDAT folding (/OPT:ICF), the D2MOO stub may be shared with other stubs returning nullptr,
		// which would get redirected too while the fixture is alive.
		d2lang_hooks.reserve(4);
		d2lang_hooks.emplace_back(moo_target, reinterpret_cast<const void*>(&D2LangStringTblMock::GetStringFromTblIndex));
		if (original_target != moo_target)
		{
			d2lang_hooks.emplace_back(original_target, reinterpret_cast<const void*>(&D2LangStringTblMock::GetStringFromTblIndex));
		}

		// Same for D2Lang.#10013, the targets already hooked above are skipped so that no function gets hooked twice
		void* original_tbl_index_target = find_imported_function_by_ordinal(original_d2common, "D2Lang.dll", D2LANG_GETTBLINDEX_ORDINAL);
		void* moo_tbl_index_target = reinterpret_cast<void*>(&D2LANG_GetTblIndex);
		const auto is_hooked = [moo_target, original_target](void* target)
		{
			return target == moo_target || target == original_target;
		};

		if (!is_hooked(moo_tbl_index_target))
		{
			d2lang_hooks.emplace_back(moo_tbl_index_target, reinterpret_cast<const void*>(&D2LangStringTblMock::GetTblIndex));
		}
		if (original_tbl_index_target != nullptr && original_tbl_index_target != moo_tbl_index_target && !is_hooked(original_tbl_index_target))
		{
			d2lang_hooks.emplace_back(original_tbl_index_target, reinterpret_cast<const void*>(&D2LangStringTblMock::GetTblIndex));
		}

		D2LangStringTblMock::calls.clear();
		D2LangStringTblMock::tbl_index_calls.clear();
	}
};
