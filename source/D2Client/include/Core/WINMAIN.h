#pragma once  
#include <D2Config.h>

#pragma pack(1)

enum D2C_AppMode {
	APPMODE_CONTINUE = 0,
	APPMODE_3 = 3,
	APPMODE_LAST = 12,
};

#pragma pack()

// Helper function
D2ConfigStrc* CONFIG_GetConfig();

//1.10f:D2Client.0x6FAA2050
int __stdcall D2ClientEntrypoint(HMODULE hModule, D2ConfigStrc* pConfig);
