#include <CGAME/scroll.h> 
#include <D2Client.h>
#include <D2Gfx.h>
#include <CGAME/View.h>

#ifdef D2_VERSION_110F
D2VAR(D2CLIENT, pgnScreenWidth, uint32_t, 0x6FB740EC - D2ClientImageBase);
D2VAR(D2CLIENT, pgnScreenHeight, uint32_t, 0x6FB740F0 - D2ClientImageBase);
D2VAR(D2CLIENT, pgdwUiNbPanels, int32_t, 0x6FBAB9C4 - D2ClientImageBase);
D2VAR(D2CLIENT, pgnDrawMaxWidth, int32_t, 0x6FB9A704 - D2ClientImageBase); // 1.13c: D2Client.0x6FBA9E14
D2VAR(D2CLIENT, pgnDrawMaxHeight, int32_t, 0x6FB9A700 - D2ClientImageBase);// 1.13c: D2Client.0x6FBA9E18
D2VAR(D2CLIENT, pgpView_6FBA7990, D2GameViewStrc*, 0x6FBA7990 - D2ClientImageBase);
D2VAR(D2CLIENT, pgbPreventBeltUsage, BOOL, 0x6FBA79C4 - D2ClientImageBase);
D2CLIENTDWORDSTUB(6FBA79A8);
D2CLIENTDWORDSTUB(6FBAB9C8);
D2CLIENTDWORDSTUB(6FBA79AC);
D2CLIENTDWORDSTUB(6FBA79C8);

//Helper function
bool UI_IsInScreen(D2CoordStrc tScreenPos)
{
	return tScreenPos.nX >= 0 && tScreenPos.nX < *D2CLIENT_pgnDrawMaxWidth
		&& tScreenPos.nY >= 0 && tScreenPos.nY < *D2CLIENT_pgnDrawMaxHeight;
}


//1.10f: D2Client.0x6FAABF60
void __fastcall sub_6FAABF60()
{
	*D2CLIENT_pdword_6FBA79A8 = 0;
	*D2CLIENT_pdword_6FBA79AC = 0;
	*D2CLIENT_pgbPreventBeltUsage = FALSE;
	*D2CLIENT_pdword_6FBA79C8 = 0;
}

//1.10f: D2Client.0x6FAB7100
void __fastcall D2Client_SetViewRect(D2GameViewStrc* pGameView, int xLeft, int yTop, int xRight, int yBottom)
{	
	if (pGameView)
	{
		SetRect(&pGameView->ViewRadius, xLeft, yTop, xRight, yBottom);
		SetRect(&pGameView->CheckRadius, -80, -80, (*D2CLIENT_pgnScreenWidth) + 80, (*D2CLIENT_pgnScreenHeight) - 47);
		if (D2GFX_CheckPerspective())
		{
			pGameView->CheckRadius.top -= 30;
			pGameView->CheckRadius.left -= 45;
			pGameView->CheckRadius.right += 30;
		}
		pGameView->dwFlags |= 1;
	}
}


//1.10f: D2Client.0x6FAB5760
void __fastcall UI_AdjustViewMatrix(D2C_GlobalUiStates nGlobalUiState)
{
	int nOffsetX = 0;
	switch (nGlobalUiState)
	{
	case UISTATE_NONE:
	case UISTATE_BOTH:
		nOffsetX = 0;
		break;
	case UISTATE_RIGHT:
		nOffsetX = *D2CLIENT_pgnScreenWidth / -4;
		break;
	case UISTATE_LEFT:
		nOffsetX = *D2CLIENT_pgnScreenWidth / 4;
		break;
	default:
		FOG_DisplayHalt("Setting scroll window mode", "C:\\projects\\D2\\head\\Diablo2\\Source\\D2Client\\GAME\\scroll.cpp", 93);
		exit(-1);
		break;
	}
	*D2CLIENT_pdword_6FBAB9C8 = nOffsetX;
	D2GFX_SetViewMatrixAdjust(nOffsetX);
	*D2CLIENT_pgdwUiNbPanels = nGlobalUiState;

	const int xLeftOffsetSize = *D2CLIENT_pgnDrawMaxWidth / 4;
	switch (nGlobalUiState)
	{
	case UISTATE_NONE:
		D2Client_SetViewRect(*D2CLIENT_pgpView_6FBA7990, 0, 0, *D2CLIENT_pgnDrawMaxWidth, *D2CLIENT_pgnDrawMaxHeight);
		break;
	case UISTATE_RIGHT:
		D2Client_SetViewRect(*D2CLIENT_pgpView_6FBA7990, -xLeftOffsetSize, 0, *D2CLIENT_pgnDrawMaxWidth - xLeftOffsetSize, *D2CLIENT_pgnDrawMaxHeight);
		break;
	case UISTATE_LEFT:
		D2Client_SetViewRect(*D2CLIENT_pgpView_6FBA7990, xLeftOffsetSize, 0, *D2CLIENT_pgnDrawMaxWidth + xLeftOffsetSize, *D2CLIENT_pgnDrawMaxHeight);
		break;
	case UISTATE_BOTH:
		D2Client_SetViewRect(*D2CLIENT_pgpView_6FBA7990, 0, 0, *D2CLIENT_pgnDrawMaxWidth, *D2CLIENT_pgnDrawMaxHeight);
		sub_6FAABF60();
		break;
	default:
		D2_UNREACHABLE;
		break;
	}

}

#endif //D2_VERSION_110F

