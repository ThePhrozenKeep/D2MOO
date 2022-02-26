#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include <D2Unicode.h>

TEST_CASE("Unicode::strncmp")
{
#define D2_USTR(widestr) (const Unicode*)widestr

    const size_t strLenDiablo2 = wcslen(L"Diablo2");

    SUBCASE("Ordering")
    {
        CHECK(Unicode::strncmp(D2_USTR(L"0"), D2_USTR(L"0"), 1) == 0);
        CHECK(Unicode::strncmp(D2_USTR(L"0"), D2_USTR(L"1"), 1) == -1);
        CHECK(Unicode::strncmp(D2_USTR(L"1"), D2_USTR(L"0"), 1) ==  1);
    }
    SUBCASE("Case sensitive")
    {
        CHECK(Unicode::strncmp(D2_USTR(L"Diablo2"), D2_USTR(L"DIABLO2"), strLenDiablo2) == 1);
        CHECK(Unicode::strncmp(D2_USTR(L"DIABLO2"), D2_USTR(L"Diablo2"), strLenDiablo2) == -1);
    }
    SUBCASE("Number of characters to compare")
    {
        CHECK(Unicode::strncmp(D2_USTR(L"Diablo2"), D2_USTR(L"Diablo2"), 0) == 0);
        CHECK(Unicode::strncmp(D2_USTR(L"Diablo2"), D2_USTR(L"Diablo2"), strLenDiablo2) == 0);
        
        CHECK(Unicode::strncmp(D2_USTR(L"Diablo2"), D2_USTR(L"Diablo3"), strLenDiablo2 - 1) == 0);
        CHECK(Unicode::strncmp(D2_USTR(L"Diablo2"), D2_USTR(L"Diablo3"), strLenDiablo2) != 0);
    }
	SUBCASE("Original game bug")
	{
        // Any string that is the prefix of the other is accepted.
        CHECK(Unicode::strncmp(D2_USTR(L"Diablo"), D2_USTR(L"Diablo2"),strLenDiablo2) == 0);
        CHECK(Unicode::strncmp(D2_USTR(L"Diablo2"), D2_USTR(L"Diablo"),strLenDiablo2) == 0);

        CHECK(Unicode::strncmp(D2_USTR(L""), D2_USTR(L"Diablo2"), strLenDiablo2) == 0);
        CHECK(Unicode::strncmp(D2_USTR(L"Diablo2"), D2_USTR(L""), strLenDiablo2) == 0);
	}
}
