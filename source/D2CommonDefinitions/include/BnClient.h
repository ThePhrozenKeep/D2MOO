#pragma once
#pragma pack(push, 1)

struct S2C_Packet_SID_LOGONREALMEX
{
	uint32_t MCPCookie;
	uint32_t MCPStatus;
	uint32_t MCPChunk1[2];
	uint32_t MCPIP;
	uint32_t MCPPort;
	uint32_t MCPChunk2[12];
	char BattleNetUniqueName[];
};

struct RealmLogonInformation
{
	uint32_t nSize;
	S2C_Packet_SID_LOGONREALMEX tPacket;
};

struct BnClientInterface
{
	void(__fastcall* Initialize)(struct D2ConfigStrc* pConfig);
	HANDLE(__stdcall* OpenBNetConnection)();
	int(__fastcall* Logon)(const char* szAccount, const char* szPassword);
	int(__stdcall* GetLogonInfo)();
	int(__stdcall* CloseBNetConnection)();
	BOOL(__thiscall* ChatCommand)(const char* _this);
	char* (__stdcall* GetUniqueName)();
	int(__fastcall* JoinChannel)(BYTE* a1, int a2, int a3);
	int(__thiscall* GetChannelList)(void* _this);
	int(__fastcall* BnMessQueue_Read)(DWORD* a1, void* a2);
	int(__thiscall* BnMessageQueueGetLastError)(DWORD* errInfo);
	int(__fastcall* field_2C)(int a1, int a2);
	int(__stdcall* CheckAd)();
	int(__fastcall* RefreshGame)(char *szGameName, char *szPassword, const char *szStatstring, int nGameType);
	void* (__fastcall* GetGamesList)(const char* szGameName, const char* szGamePassword, int a3, int a4);
	int(__fastcall* NotifyJoinedGame)(const char* szGameName, const char* szGamePassword);
	int(__stdcall* SendRealmList)();
	struct RealmLogonInformation* (__fastcall* LogonRealm)(const char *szRealmName, const char *szRealmPassword);
	int(__thiscall* PatchUnk)(DWORD* _this);
	int(__stdcall* GetPatchState)();	// patch download progress: 0..100, 101 once done
	uintptr_t(__fastcall* PatchUnk0x4C)(BYTE* source, size_t size);
	int(__fastcall* EnterChat)(const char* szName, const char* szStats, int nUnused, BYTE* szChannel, char nFlags);
	int(__stdcall* LeaveChat)();
	int(__stdcall* GetVersion)();
	int(__fastcall* CreateAccount)(int a1, int a2);
	int(__thiscall* GetCreateAccountResult)(DWORD* _this);
	int(__stdcall* IsSocketConnected)();
	int(__fastcall* OnAdClick)(int a1, int a2);
	int(__stdcall* UnkSet)();
	int(__fastcall* BattleWriteUserData)(const char* szAccount, uint32_t nKeys, const char* const* ppKeys, const char* const* ppValues);
	BOOL(__fastcall* BattleRequestReadUserData)(const char* szAccount, uint32_t nKeys, uint32_t nCookie, const char* const* ppKeys);
	int(__fastcall* GetBattleReadUserData)(uint32_t nKeys, uint32_t nCookie, char* const* ppValues, const uint32_t* pLengths);
	int(__thiscall* GetConnectionState)(DWORD* pState);
	void (*LOG_Write)(char* Format, ...);
	int(__cdecl* StartLogging)(int a1, int a2);
	int(__thiscall* GetCreateGameResult)(int* _this);
	int(__stdcall* SendKeepAlivePacket)();
	int(__stdcall* field_94)();
	struct BNGatewayAccess* (__stdcall* GetBnGatewayAccess)();
	int(__fastcall* ChangePassword)(int ecx0, int a2, int a1);
	int(__thiscall* GetChangePasswordResult)(int* _this);
	int(__thiscall* QueryAdUrl)(void* _this);
	int(__fastcall* LoadBNetNews)(int(__fastcall* a1)(DWORD, DWORD), int (*a2)(void));
	int(__stdcall* FreeBNetNews)();
	char(__stdcall* GetBuild)();
	int(__stdcall* field_B4)();
	BOOL(__thiscall* SetEmail)(const void* szEmail);
	BOOL(__fastcall* ResetPassword)(BYTE* szAccountName, BYTE* szEmailAddress);
	BOOL(__fastcall* ChangeEmail)(BYTE* a1, BYTE* a2, BYTE* a3);
	int(__stdcall* LeaveGame)();
};
#pragma pack(pop)
