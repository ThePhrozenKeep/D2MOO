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
	int(__stdcall* field0x00)();
	HANDLE(__stdcall* OpenBNetConnection)();
	int(__fastcall* Logon)(int a1, int a2);
	int(__stdcall* GetLogonInfo)();
	int(__stdcall* CloseBNetConnection)();
	BOOL(__fastcall* ChatCommand)(const char* a1);
	void* (__stdcall* field_18)();
	int(__fastcall* JoinChannel)(BYTE* a1, int a2, int a3);
	int(__fastcall* GetChannelList)(void* a1);
	int(__fastcall* BnMessQueue_Read)(DWORD* a1, void* a2);
	int(__fastcall* BnMessageQueueGetLastError)(DWORD* errInfo);
	int(__fastcall* field_2C)(int a1, int a2);
	int(__stdcall* CheckAd)();
	int(__fastcall* RefreshGame)(char *szGameName, char *szPassword, const char *szStatstring, int nGameType);
	int(__fastcall* GetGamesList)(int a1, int a2, int a3, int a4);
	int(__fastcall* NotifyJoinedGame)(int a1, int a2);
	int(__stdcall* SendRealmList)();
	struct RealmLogonInformation* (__fastcall* LogonRealm)(const char *szRealmName, const char *szRealmPassword);
	int(__fastcall* PatchUnk)(DWORD* a1);
	int(__stdcall* field_4C)();
	uintptr_t(__fastcall* PatchUnk0x4C)(BYTE* source, size_t size);
	int(__fastcall* EnterChat)(int ecx0, int a2, int a3, BYTE* a1, char a5);
	int(__stdcall* LeaveChat)();
	int(__stdcall* GetVersion)();
	int(__fastcall* CreateAccount)(int a1, int a2);
	int(__fastcall* GetCreateAccountResult)(DWORD* a1);
	int(__stdcall* field_68)();
	int(__fastcall* OnAdClick)(int a1, int a2);
	int(__stdcall* UnkSet)();
	int(__fastcall* BattleWriteUserData)(int a1, unsigned int a2, int a3, int a4);
	BOOL(__fastcall* BattleRequestReadUserData)(int a1, unsigned int a2, int a3, int a4);
	int(__fastcall* GetBattleReadUserData)(int a1, int a2, int a3, int a4);
	int(__fastcall* GetConnectionState)(DWORD* pState);
	void (*LOG_Write)(char* Format, ...);
	int(__cdecl* StartLogging)(int a1, int a2);
	int(__fastcall* GetCreateGameResult)(int* a1);
	int(__stdcall* SendKeepAlivePacket)();
	int(__stdcall* field_94)();
	struct BNGatewayAccess* (__stdcall* GetBnGatewayAccess)();
	int(__fastcall* ChangePassword)(int ecx0, int a2, int a1);
	int(__fastcall* GetChangePasswordResult)(int* a1);
	int(__fastcall* QueryAdUrl)(void* a1);
	int(__fastcall* LoadBNetNews)(int(__fastcall* a1)(DWORD, DWORD), int (*a2)(void));
	int(__stdcall* FreeBNetNews)();
	char(__stdcall* GetBuild)();
	int(__stdcall* field_B4)();
	BOOL(__fastcall* SetEmail)(const void* szEmail);
	BOOL(__fastcall* ResetPassword)(BYTE* szAccountName, BYTE* szEmailAddress);
	BOOL(__fastcall* ChangeEmail)(BYTE* a1, BYTE* a2, BYTE* a3);
	int(__stdcall* LeaveGame)();
};
#pragma pack(pop)
