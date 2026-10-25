#pragma once


#include <cstdarg>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2DataTbls.h>


template<class Fixture>
struct BooksTxtFixture : Fixture
{
	std::unique_ptr<D2BooksTxt[]> books_txt;
	int books_record_count;

	BooksTxtFixture()
	{
		const auto working_directory = std::filesystem::current_path();
		const auto d2common_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

		const auto original_sgptDataTables = reinterpret_cast<D2DataTablesStrc**>(d2common_base + 0x00096A20);

		auto [record_count, books] = read_bin_file<D2BooksTxt>(working_directory / "data/global/excel" / "Books.bin");

		sgptDataTables->pBookDataTables.nBooksTxtRecordCount = record_count;
		sgptDataTables->pBookDataTables.pBooksTxt = books.get();

		*original_sgptDataTables = sgptDataTables;

		const auto original_books = reinterpret_cast<D2BooksTxt**>(d2common_base + 0x000A9608 + 0x00000D10);
		*original_books = books.get();

		const auto original_record_count = reinterpret_cast<int*>(d2common_base + 0x000A9608 + 0x00000D0C);
		*original_record_count = record_count;

		books_txt = std::move(books);
		books_record_count = record_count;
	};
};
