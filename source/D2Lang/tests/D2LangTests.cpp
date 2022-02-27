#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include <D2Unicode.h>

#define D2_USTR(widestr) (const Unicode*)widestr

TEST_CASE("Unicode::directionality")
{
    SUBCASE("ASCII letters")
    {
        CHECK(Unicode(L'a').directionality() == Unicode::DIR_LEFT_TO_RIGHT);
        CHECK(Unicode(L'A').directionality() == Unicode::DIR_LEFT_TO_RIGHT);
    }
    SUBCASE("ASCII digits")
    {
        CHECK(Unicode(L'0').directionality() == Unicode::DIR_LEFT_TO_RIGHT);
    }
    SUBCASE("Whitespace")
    {
        CHECK(Unicode(L' ').directionality() == Unicode::DIR_NEUTRAL);
        CHECK(Unicode(L'\t').directionality() == Unicode::DIR_NEUTRAL);
        CHECK(Unicode(L'\n').directionality() == Unicode::DIR_NEUTRAL);
    }
    SUBCASE("Right-to-left")
    {
        // Hebrew
        CHECK(Unicode(0x590).directionality() == Unicode::DIR_RIGHT_TO_LEFT);
        // Right-to-Left Mark
        CHECK(Unicode(0x200F).directionality() == Unicode::DIR_RIGHT_TO_LEFT);
    }
}

TEST_CASE("Unicode::isWordEnd")
{
    SUBCASE("Index 0")
    {
        CHECK(!Unicode::isWordEnd(D2_USTR(L"0 2345678"), 0));
        CHECK(!Unicode::isWordEnd(D2_USTR(L"A soulstone"), 0));
    }
    SUBCASE("Before whitespace")
    {
        CHECK(Unicode::isWordEnd(D2_USTR(L"0123 5678"), 3));
        CHECK(Unicode::isWordEnd(D2_USTR(L"The soulstone"), 2));
    }
    SUBCASE("On whitespace")
    {
        CHECK(!Unicode::isWordEnd(D2_USTR(L"0123 5678"), 4));
        CHECK(!Unicode::isWordEnd(D2_USTR(L"The soulstone"), 3));
    }
    SUBCASE("Middle of word")
    {
        CHECK(!Unicode::isWordEnd(D2_USTR(L"0123 5678"), 2));
        CHECK(!Unicode::isWordEnd(D2_USTR(L"The soulstone"), 7));
    }
    SUBCASE("End of string")
    {
        CHECK(Unicode::isWordEnd(D2_USTR(L"0123 5678"), wcslen(L"0123 5678") - 1));
        CHECK(Unicode::isWordEnd(D2_USTR(L"The soulstone"), wcslen(L"The soulstone") - 1));
    }
}

TEST_CASE("Unicode::strcat")
{
    SUBCASE("Append empty on empty")
    {
        Unicode dest[256];
        Unicode::strcat(dest, D2_USTR(L""));
        CHECK(wcscmp((wchar_t*)dest, L"") == 0);
    }
    SUBCASE("Append text on empty")
    {
        Unicode dest[256];
        Unicode::strcat(dest, D2_USTR(L"Diablo"));
        CHECK(wcscmp((wchar_t*)dest, L"Diablo") == 0);
    }
    SUBCASE("Append text on text")
    {
        Unicode dest[256] = { L'3', L':', L' ' };
        Unicode::strcat(dest, D2_USTR(L"Mephisto, "));
        Unicode::strcat(dest, D2_USTR(L"Diablo, "));
        Unicode::strcat(dest, D2_USTR(L"and Baal"));
        CHECK(wcscmp((wchar_t*)dest, L"3: Mephisto, Diablo, and Baal") == 0);
    }
    SUBCASE("Append at correct index")
    {
        Unicode dest[256];
        Unicode::strcat(dest, D2_USTR(L"Diablo"));
        dest[3] = L'\0';
        Unicode::strcat(dest, D2_USTR(L"mond"));
        CHECK(wcscmp((wchar_t*)dest, L"Diamond") == 0);
    }
}

TEST_CASE("Unicode::strchr")
{
    const Unicode* diablo = D2_USTR(L"Diablo");

    SUBCASE("Not found in Empty")
    {
        CHECK(Unicode::strchr(D2_USTR(L""), L'D') == NULL);
        CHECK(Unicode::strchr(D2_USTR(L""), L'\0') == NULL);
    }
    SUBCASE("Found in 'Diablo'")
    {
        CHECK(Unicode::strchr(diablo, L'D') == &diablo[0]);
        CHECK(Unicode::strchr(diablo, L'a') == &diablo[2]);
        CHECK(Unicode::strchr(diablo, L'b') == &diablo[3]);
    }
    SUBCASE("Not found in 'Diablo'")
    {
        CHECK(Unicode::strchr(diablo, L'j') == NULL);
        CHECK(Unicode::strchr(diablo, 0x190) == NULL);
        CHECK(Unicode::strchr(diablo, L'\0') == NULL);
    }
}

TEST_CASE("Unicode::strcmp")
{
    SUBCASE("Ordering")
    {
        CHECK(Unicode::strcmp(D2_USTR(L"0"), D2_USTR(L"0")) == 0);
        CHECK(Unicode::strcmp(D2_USTR(L"0"), D2_USTR(L"1")) == -1);
        CHECK(Unicode::strcmp(D2_USTR(L"1"), D2_USTR(L"0")) ==  1);
    }
    SUBCASE("Case sensitive")
    {
        CHECK(Unicode::strcmp(D2_USTR(L"Diablo2"), D2_USTR(L"DIABLO2")) == 1);
        CHECK(Unicode::strcmp(D2_USTR(L"DIABLO2"), D2_USTR(L"Diablo2")) == -1);
    }
}

TEST_CASE("Unicode::strncmp")
{
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

TEST_CASE("Unicode::strlen")
{
    SUBCASE("Empty")
    {
        CHECK(Unicode::strlen(D2_USTR(L"")) == 0);
    }
    SUBCASE("Nonempty")
    {
        CHECK(Unicode::strlen(D2_USTR(L"Diablo2")) == 7);
        CHECK(Unicode::strlen(D2_USTR(L"Mephisto")) == 8);
        CHECK(Unicode::strlen(D2_USTR(L"Baal")) == 4);
    }
    SUBCASE("Null")
    {
        CHECK(Unicode::strlen(nullptr) == 0);
    }
}
