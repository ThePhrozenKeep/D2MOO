#pragma once

template<class Fixture>
struct UnitWithStatFlagsFixture : Fixture
{
	D2UnitStrc moo_pUnit{};
	D2StatListExStrc moo_pStatListEx{};
	std::unique_ptr<uint32_t[]> moo_StatFlags;

	D2UnitStrc original_pUnit{};
	D2StatListExStrc original_pStatListEx{};
	std::unique_ptr<uint32_t[]> original_StatFlags;

	int flag_count;

	UnitWithStatFlagsFixture()
	{
		flag_count = (states_record_count + 31) / 32;

		moo_StatFlags = std::make_unique<uint32_t[]>(2 * flag_count);
		original_StatFlags = std::make_unique<uint32_t[]>(2 * flag_count);

		const auto setup_data = [this](
			D2UnitStrc& pUnit,
			D2StatListExStrc& pStatListEx,
			std::unique_ptr<uint32_t[]>& StatFlags
		) {
			pStatListEx.dwFlags |= STATLIST_EXTENDED;
			pStatListEx.StatFlags = StatFlags.get();
			pUnit.pStatListEx = &pStatListEx;
		};

		setup_data(moo_pUnit, moo_pStatListEx, moo_StatFlags);
		setup_data(original_pUnit, original_pStatListEx, original_StatFlags);

		reroll_flags();
	}

	void reroll_flags()
	{
		for (auto i = 0; i < 2 * flag_count; ++i)
		{
			const auto new_flag = random_unsigned_integer();
			moo_StatFlags[i] = new_flag;
			original_StatFlags[i] = new_flag;
		}
	}
};
