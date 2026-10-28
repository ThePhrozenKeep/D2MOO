#pragma once


#define REPEAT_5() [[maybe_unused]] const auto repetition = GENERATE(1, 2, 3, 4, 5);
#define REPEAT_10() [[maybe_unused]] const auto repetition = GENERATE(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
#define REPEAT_20() [[maybe_unused]] const auto repetition = GENERATE(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20);
