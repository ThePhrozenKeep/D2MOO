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
	int32_t(__fastcall* Logon)(const char* szAccount, const char* szPassword);
	int32_t(__stdcall* GetLogonInfo)();
	BOOL(__stdcall* CloseBNetConnection)();
	BOOL(__thiscall* ChatCommand)(const char* _this);
	char* (__stdcall* GetUniqueName)();
	BOOL(__fastcall* JoinChannel)(const char* szChannel, BOOL bForce, BOOL bOpen);
	const void* (__thiscall* GetChannelList)(BOOL bOpen);
	int32_t(__fastcall* BnMessQueue_Read)(DWORD* pType, void* pMessage);
	BOOL(__thiscall* BnMessageQueueGetLastError)(DWORD* errInfo);
	BOOL(__fastcall* field_2C)(const char* szFileName, uint32_t nRequestId);
	BOOL(__stdcall* CheckAd)();
	BOOL(__fastcall* RefreshGame)(const char* szGameName, const char* szPassword, const char* szStatstring, int32_t nGameType);
	void* (__fastcall* GetGamesList)(const char* szGameName, const char* szGamePassword, int32_t nUnused1, int32_t nUnused2);
	BOOL(__fastcall* NotifyJoinedGame)(const char* szGameName, const char* szGamePassword);
	const void* (__stdcall* SendRealmList)();
	struct RealmLogonInformation* (__fastcall* LogonRealm)(const char *szRealmName, const char *szRealmPassword);
	BOOL(__thiscall* PatchUnk)(DWORD* _this);
	int32_t(__stdcall* GetPatchState)();	// patch download progress: 0..100, 101 once done
	uintptr_t(__fastcall* PatchUnk0x4C)(const char* szFileName, size_t size);
	BOOL(__fastcall* EnterChat)(const char* szName, const char* szStats, int32_t nUnused, const char* szChannel, char nFlags);
	BOOL(__stdcall* LeaveChat)();
	int32_t(__stdcall* GetVersion)();
	int32_t(__fastcall* CreateAccount)(const char* szAccount, const char* szPassword);
	int32_t(__thiscall* GetCreateAccountResult)(DWORD* _this);
	BOOL(__stdcall* IsSocketConnected)();
	BOOL(__fastcall* OnAdClick)(uint32_t nAdId, BOOL a2);
	void(__stdcall* UnkSet)();
	int32_t(__fastcall* BattleWriteUserData)(const char* szAccount, uint32_t nKeys, const char* const* ppKeys, const char* const* ppValues);
	BOOL(__fastcall* BattleRequestReadUserData)(const char* szAccount, uint32_t nKeys, uint32_t nCookie, const char* const* ppKeys);
	int32_t(__fastcall* GetBattleReadUserData)(uint32_t nKeys, uint32_t nCookie, char* const* ppValues, const uint32_t* pLengths);
	int32_t(__thiscall* GetConnectionState)(DWORD* pState);
	void (*LOG_Write)(const char* Format, ...);
	BOOL(__cdecl* StartLogging)(int32_t a1, int32_t a2);
	int32_t(__thiscall* GetCreateGameResult)(int32_t* _this);
	BOOL(__stdcall* SendKeepAlivePacket)();
	int32_t(__stdcall* field_94)();
	struct BNGatewayAccess* (__stdcall* GetBnGatewayAccess)();
	int32_t(__fastcall* ChangePassword)(const char* szAccount, const char* szOldPassword, const char* szNewPassword);
	int32_t(__thiscall* GetChangePasswordResult)(int32_t* _this);
	const char* (__thiscall* QueryAdUrl)(uint32_t nAdId);
	BOOL(__fastcall* LoadBNetNews)(int32_t(__fastcall* a1)(DWORD, DWORD), int32_t (*a2)(void));
	BOOL(__stdcall* FreeBNetNews)();
	char(__stdcall* GetBuild)();
	int32_t(__stdcall* field_B4)();
	BOOL(__thiscall* SetEmail)(const char* szEmail);
	BOOL(__fastcall* ResetPassword)(const char* szAccountName, const char* szEmailAddress);
	BOOL(__fastcall* ChangeEmail)(const char* szAccountName, const char* szOldEmail, const char* szNewEmail);
	BOOL(__stdcall* LeaveGame)();
};
#pragma pack(pop)
