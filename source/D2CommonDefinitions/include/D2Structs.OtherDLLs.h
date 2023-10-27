
// Archive of types that are probably coming from other DLLs.

// ---------- D2Client ----------

struct D2AltDrawStrc
{
	int32_t nLeft;								//0x000
	int32_t nTop;								//0x004
	int32_t nRight;								//0x008
	int32_t nBottom;							//0x00C
	D2UnitStrc* pItem;							//0x010
	wchar_t wszName[128];						//0x014
	int32_t nRectColor;							//0x114
	int32_t nRectDrawMode;						//0x118
	int32_t nColor;								//0x11C
};

struct D2AnvilUIButtonStrc
{
	uint16_t nStringId;							//0x00
	int32_t nTextX;								//0x02
	int32_t nTextY;								//0x06
	int32_t nCellfileX;							//0x0A
	int32_t nCellfileY;							//0x0E
	int32_t nClickAreaXStart;					//0x12
	int32_t nClickAreaXEnd;						//0x16
	int32_t nClickAreaYStart;					//0x1A
	int32_t nClickAreaYEnd;						//0x1E
	int32_t nFrame;								//0x22
	uint8_t bButtonClicked;						//0x26
	uint8_t unk0x27;							//0x27
};

struct D2AutomapCellStrc
{
	uint32_t fSaved;							//0x00
	uint16_t nCellNo;							//0x04
	uint16_t xPixel;							//0x06
	uint16_t yPixel;							//0x08
	uint16_t wWeight;							//0x0A
	D2AutomapCellStrc* pPrev;				//0x0C
	D2AutomapCellStrc* pNext;				//0x10
};

struct D2AutomapLayerStrc
{
	uint32_t nLayerNo;							//0x00
	uint32_t fSaved;							//0x04
	D2AutomapCellStrc* pFloors;				//0x08
	D2AutomapCellStrc* pWalls;				//0x0C
	D2AutomapCellStrc* pObjects;			//0x10
	D2AutomapCellStrc* pExtras;				//0x14
	D2AutomapLayerStrc* pNext;				//0x18
};


struct D2BuySellTabStrc
{
	int32_t nX;									//0x00
	int32_t nY;									//0x04
	uint16_t nStringIndex;						//0x06
	BOOL bSelected;							//0x0A
	BOOL bActive;							//0x0E
};


struct D2KeyConfigOptionStrc
{
	uint16_t nStringIndex;					//0x00
	void* pCallback;						//0x02
	WORD word0x06;							//0x06
	int nTextBeginX;						//0x08
	int nTextEndX;							//0x0C
	int nTextBeginY;						//0x10
	int nTextEndY;							//0x14
	WORD word0x18;							//0x18
};

struct D2KeyConfigStrc
{
	int32_t nConfig;							//0x00
	uint16_t nStringIndex;						//0x04
	int32_t unk0x06;							//0x06
};



typedef int32_t(__fastcall* MENUENABLED)();
typedef void(__fastcall* MENUSELECT)(D2MenuItemStrc*, D2WinMsgStrc*);
typedef void(__fastcall* MENUOPTION)(D2MenuItemStrc*, int32_t);
typedef void(__fastcall* MENUUPDATE)(D2MenuItemStrc*, int32_t);

struct D2MenuEntryStrc
{
	int32_t nType;								//0x00
	int32_t nLeft;								//0x04
	int32_t nTop;								//0x08
	int32_t nWidth;								//0x0C
	int32_t nHeight;							//0x10
	int32_t unk0x014;								//0x14
	int32_t nStrIndex;							//0x18
	D2GfxDataStrc* pGfxData;				//0x1C
	int32_t(__stdcall* pBtnFunction)(void*);	//0x20
	uint32_t unk0x024[2];							//0x24
	int32_t nFont;								//0x2C
};

struct D2MenuInfoStrc
{
	int32_t nItemCount;							//0x00
	int32_t unk0x04;							//0x04
	int32_t unk0x08;							//0x08
	int32_t unk0x0C;							//0x0C
	int32_t unk0x10;							//0x10
	int32_t unk0x14;							//0x14
};

struct D2MenuItemStrc
{
	uint32_t dwType;							//0x00
	BOOL bExpansion;						//0x04
	int32_t nHeight;							//0x08
	char szImage[260];						//0x0C
	MENUENABLED pfEnabled;					//0x110
	MENUSELECT pfSelect;					//0x114			
	MENUOPTION pfOption;					//0x118
	MENUUPDATE pfUpdate;					//0x11C
	uint32_t dwCount;							//0x120
	uint32_t dwMoveCount;						//0x124
	uint32_t dwSliderType;						//0x128
	char szChoices[4][260];					//0x12C
	D2CellFileStrc* pImage;					//0x53C
	D2CellFileStrc* pChoices[4];			//0x540
};

struct D2MenuUIStateSaveStrc
{
	BOOL bCloseWhenOpen;					//0x00
	BOOL bSaveUIState;						//0x04
	int32_t nUIState;							//0x08
};


struct D2QuestDescStrc
{
	uint16_t wTblTitle;							//0x00
	uint16_t wTblInitDesc;						//0x02
};

struct D2QuestDescriptorStrc
{
	uint8_t bActive;							//0x00
	uint8_t nQuestNo;							//0x01
	uint8_t nPosition;							//0x02
	uint8_t nTab;								//0x03
	uint16_t* pStringIds;						//0x04
	int32_t nQuestFlag;							//0x08
	int32_t nArrayId;							//0x0C
};

struct D2QuestUiButtonCoordStrc
{
	int32_t nCellfileX;							//0x00
	int32_t nCellfileY;							//0x04
	int32_t nClickX;							//0x08
	int32_t nClickY;							//0x0C
};

struct D2QuestUiStrc
{
	uint8_t field_0;							//0x00
	int32_t nQuest;								//0x01
	int16_t nQuestTitleStringId;				//0x05
	wchar_t wszQuestDesc[300];				//0x07
	int16_t field_25F;						//0x25F
	uint8_t nPosition;							//0x261
	uint8_t nQuestNo;							//0x262
	uint8_t field_263;							//0x263
	uint8_t field_264;							//0x264
	uint8_t field_265;							//0x265
	int32_t nQuestState;						//0x266
};

struct D2QuestUiTabStrc
{
	int32_t nStartQuest;						//0x00
	int32_t nEndQuest;							//0x04
};


struct D2WindowPlacementStrc
{
	HWND hWnd;								//0x00
	WINDOWPLACEMENT windowPlacement;		//0x04
};

struct D2WinMsgStrc
{
	HWND hWnd;								//0x00
	uint32_t uMessage;							//0x04
	union									//0x08
	{
		WPARAM  wParam;
		int32_t	  nKey;
	};
	union									//0x0C
	{
		struct
		{
			uint16_t	nXpos;
			uint16_t	nYpos;
		};
		LPARAM   lParam;
	};
	uint32_t dwCommandSource;					//0x10 HIWORD(wParam) when uMessage == WM_COMMAND
	uint32_t dwArg;							//0x14
	BOOL bReturn;							//0x18
	LRESULT lResult;						//0x1C
};

// ---------- D2Launch ----------

struct D2BnetClientDataStrc
{
	uint32_t dwExpansion;						//0x000
	uint8_t nWindowed;							//0x004
	uint8_t nFixAspectRatio;					//0x005
	uint8_t n3DFXMode;							//0x006
	uint8_t nOpenGLMode;						//0x007
	uint8_t nRaveMode;							//0x008
	uint8_t nDirect3DMode;						//0x009
	uint8_t nUsePerspective;					//0x00A
	uint8_t nLowQuality;						//0x00B
	uint32_t dwGamma;							//0x00C
	uint8_t nVSync;							//0x010
	uint32_t dwFrameRate;						//0x011
	uint32_t dwGameType;						//0x015
	uint16_t nJoinId;							//0x019
	char szGameName[24];					//0x01B
	char szServerAddress[24];				//0x033
	char szBattleNetAddress[24];			//0x04B
	char szMCPAddress[24];					//0x063
	uint32_t unk0x07B;							//0x07B
	uint8_t nNoPK;								//0x07F
	uint8_t nOpenCharacter;					//0x080
	uint8_t nClasses[7];						//0x081
	uint8_t nInvincible;						//0x088
	char szAccount[48];						//0x089
	char szCharName[24];					//0x0B9
	char szRealmName[32];					//0x0D1
	uint8_t unk0x0F1[249];						//0x0F1
	uint8_t nCharClass;						//0x1EA
	uint8_t nCharFlags;						//0x1EB
	uint8_t nLastCompleteDiff;					//0x1EC
	uint8_t nNoMonsters;						//0x1ED
	uint8_t __1ED[23];							//0x1EE
	//uint8_t bDirect;							//0x200
	//uint8_t bNoCompress;						//0x202
	uint32_t dwArenaFlags;						//0x205
	uint8_t nTemplate;							//0x209
	uint16_t __20A;								//0x20A
	uint8_t nDifficulty;						//0x20C
	uint8_t __20D[52];							//0x20D
	//uint8_t bNoSound;						//0x21C
	char szGamePassword[24];				//0x241
	char szGameDesc[32];					//0x259
	uint8_t __279[226];						//0x279
	char szChannel[32];						//0x35B
	uint8_t __37B[64];							//0x37B
	uint8_t nCharacterLevel;					//0x3BB
	uint8_t nLadder;							//0x3BC
	uint32_t dwPasswordHash;					//0x3BD
	uint8_t nPasswordLength;					//0x3C1
	uint8_t __3C2[6];							//0x3C2
};
