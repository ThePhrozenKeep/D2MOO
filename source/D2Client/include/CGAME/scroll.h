#pragma once  

#include <D2Constants.h>
#include <D2BasicTypes.h>

//Helper function
bool UI_IsInScreen(D2CoordStrc tScreenPos);

//1.10f: D2Client.0x6FAB5760
void __fastcall UI_AdjustViewMatrix(D2C_GlobalUiStates nGlobalUiState);

