#pragma once  
#include <D2CommonDefinitions.h>
#include <D2Constants.h>

#pragma pack(1)


#pragma pack()

//D2Client.0x6FB23800
BOOL __fastcall CLIENT_UpdateUIs_6FB23800(int a1, int a2);
//D2CLient.0x6FB23860
int __fastcall D2CLIENT_CheckUIState_6FB23860(D2C_UIIds nUIId);
//D2CLient.0x6FB23260
BOOL __fastcall D2CLIENT_UI_Handler_6FB23260(D2C_UIIds nUIId, D2C_UiStates nUIState, BOOL a3);
