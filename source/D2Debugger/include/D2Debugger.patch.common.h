#include <chrono>
#include <thread>
#include <type_traits>

#include <D2Debugger.h>
#include <GAME/Game.h>


extern decltype(&GAME_UpdateProgress) GAME_UpdateProgress_Original;
void __fastcall GAME_UpdateProgress_WithDebugger(D2GameStrc* pGame);
