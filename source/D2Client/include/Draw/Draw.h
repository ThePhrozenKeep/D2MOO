#pragma once

#include <D2BasicTypes.h>
#include <D2Config.h>
#include <DisplayType.h>

//1.10f: 0x6FB6B7F6 (thunk for D2GFX_SetGamma)
void __fastcall DRAW_SetGamma(uint32_t nGamma);


//1.10f: D2Client.0x6FAABD30
void __fastcall CONFIG_Apply_GDI(D2ConfigStrc* pConfig);

//1.10f: D2Client.0x6FAABD40
void __fastcall CONFIG_Apply_Glide(D2ConfigStrc* pConfig);

//1.10f: D2Client.0x6FAABD50
void __fastcall CONFIG_Apply_OpenGL(D2ConfigStrc* pConfig);

//1.10f: D2Client.0x6FAABD60
void __fastcall CONFIG_Apply_D3D(D2ConfigStrc* pConfig);

//1.10f: D2Client.0x6FAABD70
void __fastcall CONFIG_Apply_Rave(D2ConfigStrc* pConfig);

//1.10f: D2Client.0x6FAA28C0
DisplayType __fastcall CLIENT_GetDisplayType();

//1.10f: D2Client.0x6FAA28D0
BOOL __fastcall CLIENT_IsDisplayType3D();

//1.10f: D2Client.0x6FAABD80
void __fastcall CONFIG_Apply_Gamma(D2ConfigStrc* pConfig);

//1.10f: D2Client.0x6FAABD90
void __fastcall CONFIG_Apply_VSync(D2ConfigStrc* pConfig);