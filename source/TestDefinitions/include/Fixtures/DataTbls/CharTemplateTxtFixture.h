#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct CharTemplateTxtFixture : Fixture
{
	std::unique_ptr<D2CharTemplateTxt[]> chartemplate_txt;
	int chartemplate_record_count;

	CharTemplateTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		auto [record_count, chartemplate] = read_bin_file<D2CharTemplateTxt>(working_directory / "data/global/excel" / "CharTemplate.bin");

		gnCharTemplateTxtTableRecordCount = record_count;
		gpCharTemplateTxtTable = chartemplate.get();

		const auto original_chartemplate = reinterpret_cast<D2CharTemplateTxt**>(d2common_base + 0x000A95F8);
		*original_chartemplate = chartemplate.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A95FC);
		*original_record_count = record_count;

		chartemplate_txt = std::move(chartemplate);
		chartemplate_record_count = record_count;

		std::memset(gnCharTemplateStartIds, 0, sizeof(gnCharTemplateStartIds));

		auto nMaxLevel = 0;
		for (auto i = 0; i < gnCharTemplateTxtTableRecordCount; ++i)
		{
			const auto nLevel = gpCharTemplateTxtTable[i].nLevel;
			if (nLevel > nMaxLevel)
			{
				gnCharTemplateStartIds[nLevel] = i;
				nMaxLevel = nLevel;
			}
		}

		const auto original_template_start_ids = reinterpret_cast<int*>(d2common_base + 0x000A94F8);
		std::memcpy(original_template_start_ids, gnCharTemplateStartIds, sizeof(gnCharTemplateStartIds));
	};
};
