#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>


#include <Path/Step.h>

TEST_CASE("PATH_GetDirectionVector preserves native overflow behavior only with NO_BUG_FIX")
{
    D2CoordStrc vector = {};
    int direction = 0;
    PATH_GetDirectionVector(&vector, &direction, 14803u << 16, 5113u << 16, 10004u << 16, 8003u << 16);
#ifdef NO_BUG_FIX
#if D2_VERSION_MAJOR >= 1 && D2_VERSION_MINOR >= 14
    CHECK(direction == 10);
    CHECK(vector.nX == -0x20327468);
    CHECK(vector.nY == 12);
#else
    CHECK(direction == 23);
    CHECK(vector.nX == -6);
    CHECK(vector.nY == 6);
#endif
#else
    D2CoordStrc reference = {};
    int referenceDirection = 0;
    PATH_GetDirectionVector(&reference, &referenceDirection, 4799, 0, 0, 2890);
    CHECK(direction == referenceDirection);
    CHECK(vector.nX == reference.nX);
    CHECK(vector.nY == reference.nY);
#endif
}

#ifndef NO_BUG_FIX
TEST_CASE("PATH_GetDirectionVector is independent of distance scale")
{
    // Cover every octant, the signed boundary, and positive as well as negative wrapped products.
    const DWORD distances[][2] = { { 1, 0 }, { 0, 1 }, { 1, 1 }, { 4799, 2890 }, { 2890, 4799 }, { 600, 1000 }, { 32768, 65535 }, { 65535, 32768 } };
    for (const auto& distance : distances)
    {
        for (int quadrant = 0; quadrant < 4; ++quadrant)
        {
            const DWORD startX = (quadrant & 1) ? distance[0] : 0;
            const DWORD targetX = (quadrant & 1) ? 0 : distance[0];
            const DWORD startY = (quadrant & 2) ? distance[1] : 0;
            const DWORD targetY = (quadrant & 2) ? 0 : distance[1];
            D2CoordStrc reference = {}, scaled = {};
            int referenceDirection = 0, scaledDirection = 0;
            PATH_GetDirectionVector(&reference, &referenceDirection, startX, startY, targetX, targetY);
            PATH_GetDirectionVector(&scaled, &scaledDirection, startX << 16, startY << 16, targetX << 16, targetY << 16);
            CHECK(scaledDirection == referenceDirection);
            CHECK(scaled.nX == reference.nX);
            CHECK(scaled.nY == reference.nY);
        }
    }
}

TEST_CASE("PATH_GetDirectionVector handles coincident points and the full DWORD range")
{
    const DWORD endpoints[][2] = { { 0, 0 }, { 0xFFFFFFFFu, 0 }, { 0, 0xFFFFFFFFu }, { 0xFFFFFFFFu, 0xFFFFFFFFu } };
    for (const auto& endpoint : endpoints)
    {
        D2CoordStrc reference = {}, vector = {};
        int referenceDirection = 0, direction = 0;
        PATH_GetDirectionVector(&reference, &referenceDirection, 0, 0, endpoint[0] ? 1 : 0, endpoint[1] ? 1 : 0);
        PATH_GetDirectionVector(&vector, &direction, 0, 0, endpoint[0], endpoint[1]);
        CHECK(direction == referenceDirection);
        CHECK(vector.nX == reference.nX);
        CHECK(vector.nY == reference.nY);
    }
}
#endif
