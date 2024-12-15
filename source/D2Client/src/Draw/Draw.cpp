#include <Draw/Draw.h>
#include <D2Client.h>
#include <D2Gfx.h>

#ifdef D2_VERSION_110F

D2VAR(D2CLIENT, pgDisplayType, DisplayType, 0x6FB74970 - D2ClientImageBase);

//1.10f: D2Client.0x6FB9A968
static BOOL gbAskedForGDI = FALSE; // Unused ?
//1.10f: D2Client.0x6FAABD30
void __fastcall CONFIG_Apply_GDI(D2ConfigStrc* pConfig)
{
	if (pConfig->bWindow)
	{
		*D2CLIENT_pgDisplayType = DISPLAYTYPE_GDI;
		gbAskedForGDI = TRUE;
	}
}


//1.10f: D2Client.0x6FAABD40
void __fastcall CONFIG_Apply_Glide(D2ConfigStrc* pConfig)
{
	if (pConfig->b3DFX) *D2CLIENT_pgDisplayType = DISPLAYTYPE_GLIDE;
}
//1.10f: D2Client.0x6FAABD50
void __fastcall CONFIG_Apply_OpenGL(D2ConfigStrc* pConfig)
{
	if (pConfig->bOpenGL) *D2CLIENT_pgDisplayType = DISPLAYTYPE_OPENGL;
}
//1.10f: D2Client.0x6FAABD60
void __fastcall CONFIG_Apply_D3D(D2ConfigStrc* pConfig)
{
	if (pConfig->bD3D) *D2CLIENT_pgDisplayType = DISPLAYTYPE_DIRECT3D;
}

//1.10f: D2Client.0x6FAABD70
void __fastcall CONFIG_Apply_Rave(D2ConfigStrc* pConfig)
{
	if (pConfig->bRave) *D2CLIENT_pgDisplayType = DISPLAYTYPE_RAVE;
}

//1.10f: D2Client.0x6FAA28C0
DisplayType __fastcall CLIENT_GetDisplayType()
{
	return *D2CLIENT_pgDisplayType;
}

//1.10f: D2Client.0x6FAA28D0
BOOL __fastcall CLIENT_IsDisplayType3D()
{
	return *D2CLIENT_pgDisplayType >= DISPLAYTYPE_GLIDE;
}

//1.10f: D2Client.0x6FAABD80
void __fastcall CONFIG_Apply_Gamma(D2ConfigStrc* pConfig)
{
	if (pConfig->dwGamma != 0) DRAW_SetGamma(pConfig->dwGamma);
}

//1.10f: D2Client.0x6FAABD90
void __fastcall CONFIG_Apply_VSync(D2ConfigStrc* pConfig)
{
	if (pConfig->bVSync) D2GFX_EnableVSync();
}

void __fastcall DRAW_SetGamma(uint32_t nGamma)
{
	D2GFX_SetGamma(nGamma);
}


#endif