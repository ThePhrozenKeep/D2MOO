#pragma once 
#include <D2Dll.h>
#include "StormHandles.h"

#ifdef STORM_IMPL
#define STORM_DLL_DECL // We use .def files, not dllexport
#else
#define STORM_DLL_DECL __declspec( dllimport )
#endif

// Version 1.10f:
// List of Storm.dll exports (base address 6ffb0000)
// For each of the other .DLLs, here are the number of imports of the current module exports:
// (Note that this is not the number of uses/references, just the number of times the current module is listed in the import table)
// |- D2gfx.dll        :   1
// |- D2Launch.dll     :  19
// |- D2Direct3D.dll   :   7
// |- D2OpenGL.dll     :   2
// |- D2Gdi.dll        :   3
// |- D2Glide.dll      :   5
// |- Fog.dll          :  31
// |- D2MCPClient.dll  :   2
// |- D2CMP.dll        :   7
// |- D2Common.dll     :   4
// |- D2Multi.dll      :   8
// |- D2DDraw.dll      :   7
// |- D2sound.dll      :   8
// |- D2Win.dll        :  17
// |- D2Game.dll       :   8
// |- D2Client.dll     :  35
// |- D2Lang.dll       :   4

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetCreateGame, BOOL, __stdcall, (char *Source, char *a2, char *a3, int a4, int a5, int a6, char *a7, char *a8, int *playerid), 0x1dbb0);  //Storm.#101

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetDestroy, BOOL, __cdecl, (), 0x1e3a0);  //Storm.#102

/// Not imported by any .dll
using SNetEnumProviders_Callback = int(__stdcall*)(DWORD, DWORD, DWORD, DWORD);
D2FUNC_DLL_NP(STORM, SNetEnumProviders, BOOL, __stdcall, (int mincaps, SNetEnumProviders_Callback pCallback), 0x1ec30);  //Storm.#103

/// Not imported by any .dll
using SNetEnumDevices_Callback = int(__stdcall* )(DWORD, DWORD, DWORD);
D2FUNC_DLL_NP(STORM, SNetEnumDevices, BOOL, __stdcall, (SNetEnumDevices_Callback pCallback), 0x1ed60);  //Storm.#104

/// Not imported by any .dll
using SNetEnumGames_Callback = int(__stdcall*)(DWORD, DWORD, DWORD);
D2FUNC_DLL_NP(STORM, SNetEnumGames, BOOL, __stdcall, (SNetEnumGames_Callback pCallback, int *hintnextcall), 0x1f0a0);  //Storm.#105

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetDropPlayer, BOOL, __stdcall, (int playerid, int flags), 0x1e670);  //Storm.#106

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetGetGameInfo, BOOL, __stdcall, (int type, void *src, unsigned int length, int *byteswritten), 0x1f6b0);  //Storm.#107

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetGetNetworkLatency, BOOL, __stdcall, (int measurementtype, int *result), 0x1f820);  //Storm.#108

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetGetNumPlayers, BOOL, __stdcall, (int *firstplayerid, int *lastplayerid, int *activeplayers), 0x1f910);  //Storm.#109

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetGetOwnerTurnsWaiting, BOOL, __stdcall, (int *turns), 0x1fa40);  //Storm.#110

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetGetPerformanceData, int, __stdcall, (int, int, int, int, LPFILETIME lpFileTime, int), 0x20120);  //Storm.#111

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetGetPlayerCaps, BOOL, __stdcall, (char a1, int *caps), 0x20210);  //Storm.#112

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetGetPlayerName, BOOL, __stdcall, (int playerid, char *buffer, size_t buffersize), 0x20320);  //Storm.#113

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetGetProviderCaps, BOOL, __stdcall, (void *caps), 0x20410);  //Storm.#114

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetGetTurnsInTransit, BOOL, __stdcall, (int *turns), 0x204b0);  //Storm.#115

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetInitializeDevice, BOOL, __stdcall, (int a1, int a2, int a3, int a4, int a5), 0x20580);  //Storm.#116

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetInitializeProvider, BOOL, __stdcall, (DWORD providerName, int a2, int a3, int a4, DWORD ThreadID), 0x208a0);  //Storm.#117

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetJoinGame, BOOL, __stdcall, (int a1, char *a2, char *a3, char *playerName, char *a5, int *playerid), 0x22a20);  //Storm.#118

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetLeaveGame, BOOL, __stdcall, (int type), 0x23090);  //Storm.#119

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetPerformUpgrade, BOOL, __stdcall, (DWORD *upgradestatus), 0x23270);  //Storm.#120

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetReceiveMessage, BOOL, __stdcall, (int *senderplayerid, int *data, int *databytes), 0x23a20);  //Storm.#121

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetReceiveTurns, BOOL, __stdcall, (unsigned int a1, int arraysize, void *arraydata, void *arraydatabytes, void *arrayplayerstatus), 0x23cd0);  //Storm.#122

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetRegisterEventHandler, HANDLE, __stdcall, (int event, int size), 0x24330);  //Storm.#123

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetResetLatencyMeasurements, BOOL, __cdecl, (), 0x24390);  //Storm.#124

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetSelectGame, int, __stdcall, (int a1, int a2, int a3, int a4, int a5, int *playerid), 0x244e0);  //Storm.#125

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetSelectProvider, BOOL, __stdcall, (LPARAM lparam, int a2, int a3, int a4, int a5, int *providerid), 0x245b0);  //Storm.#126

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetSendMessage, BOOL, __stdcall, (signed int playerID, void *data, int databytes), 0x24f10);  //Storm.#127

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetSendTurn, BOOL, __stdcall, (void *data, int databytes), 0x251b0);  //Storm.#128

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetSetBasePlayer, BOOL, __stdcall, (int a1), 0x252f0);  //Storm.#129

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetSetGameMode, BOOL, __stdcall, (int a1), 0x25320);  //Storm.#130

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetUnregisterEventHandler, int, __stdcall, (int a1, int callback), 0x25410);  //Storm.#131

/// Not imported by any .dll
using SNetEnumGamesEx_Callback = int(__fastcall*)(DWORD, DWORD, DWORD);
D2FUNC_DLL_NP(STORM, SNetEnumGamesEx, BOOL, __stdcall, (int a1, int a2, SNetEnumGamesEx_Callback pCallback, int *hintnextcall), 0x1ef60);  //Storm.#133

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetSendServerChatCommand, int, __stdcall, (char *command), 0x25090);  //Storm.#134

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetSend135, BOOL, __stdcall, (signed int a1, int a2, int a3), 0x24d90);  //Storm.#135

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetReceive136, BOOL, __stdcall, (int a1, int a2, int a3), 0x237a0);  //Storm.#136

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetGetPlayerNames, BOOL, __stdcall, (int a1), 0x1e5d0);  //Storm.#137

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetCreateLadderGame, BOOL, __stdcall, (int a1, int a2, int a3, int a4, int a5, int a6, const void *MemSrc, int MemSize, int a9, char *playerName, int a11, int a12), 0x1dbf0);  //Storm.#138

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetReportGameResult, BOOL, __stdcall, (unsigned int a1, int size, int a3, int a4, int a5), 0x243e0);  //Storm.#139

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SNetCheckDataFile, BOOL, __stdcall, (int a1, int a2, int a3, int a4), 0x1db00);  //Storm.#140

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgBeginPaint, int, __stdcall, (HWND hWnd, LPPAINTSTRUCT lpPaint), 0xcfb0);  //Storm.#201

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgBltToWindowI, int, __stdcall, (HWND hWnd, HRGN hrgnSrc2, int, int, int, int, int, int, int, int), 0xdb30);  //Storm.#202

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgCheckTimers, BOOL, __cdecl, (), 0xdba0);  //Storm.#203

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgCreateDialogIndirectParam, int, __stdcall, (int, int, HWND hWnd, LPVOID lpParam, LPARAM lParam), 0xdc30);  //Storm.#204

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgCreateDialogParam, int, __stdcall, (HMODULE hModule, LPCSTR lpName, HWND hWnd, LPVOID lpParam, LPARAM lParam), 0xf790);  //Storm.#205

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgDefDialogProc, int, __stdcall, (HWND, int, HDC hdc, HWND hWnd), 0xf800);  //Storm.#206

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgDialogBoxIndirectParam, int, __stdcall, (int, int, HWND hWnd, LPVOID lpParam, LPARAM lParam), 0xf980);  //Storm.#208

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgDialogBoxParam, int, __stdcall, (HMODULE hModule, LPCSTR lpName, HWND hWnd, LPVOID lpParam, LPARAM lParam), 0xfa70);  //Storm.#209

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgDrawBitmap, int, __stdcall, (HWND hWnd, int, HRGN hrgnSrc2, int, int, int, char), 0xfab0);  //Storm.#210

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgEndDialog, int, __stdcall, (HWND hWnd, HANDLE hData), 0xfcb0);  //Storm.#211

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgEndPaint, BOOL, __stdcall, (HWND hWnd, PAINTSTRUCT *lpPaint), 0xfcf0);  //Storm.#212

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgKillTimer, int, __stdcall, (int a1, int a2), 0xfd20);  //Storm.#213

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgSetBaseFont, int, __stdcall, (int a1, int a2, int a3, int a4, char *Source), 0xfda0);  //Storm.#214

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgSetBitmapI, int, __stdcall, (int, int, char *Str2, int, int, int, int, int, int, int), 0x100f0);  //Storm.#215

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgSetControlBitmaps, int, __stdcall, (HWND hDlg, int, int, int, int, int, int), 0x10160);  //Storm.#216

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgSetCursor, int, __stdcall, (HWND hWnd, LONG dwNewLong, int, int), 0x10310);  //Storm.#217

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgSetSystemCursor, int, __stdcall, (int a1, int a2, int a3, signed int a4), 0x10420);  //Storm.#218

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgSetTimer, int, __stdcall, (int a1, int a2, int a3, int a4), 0x105d0);  //Storm.#219

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgUpdateCursor, int, __cdecl, (), 0x10680);  //Storm.#220

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgBltToWindowE, int, __stdcall, (HWND hWnd, HRGN hrgnSrc2, int, int, int, int, int, int, int, int), 0xd1e0);  //Storm.#221

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgSetBitmapE, int, __stdcall, (int, int, char *Str2, int, int, int, int, int, int, int), 0xfe30);  //Storm.#222

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDlgSetLocale, char*, __stdcall, (int a1, const char *a2), 0x10400);  //Storm.#223

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SFileAuthenticateArchive, int, __stdcall, (HANDLE hArchive, int a2), 0x13b00);  //Storm.#251

/// Imported by ['D2Win.dll']
D2FUNC_DLL_NP(STORM, SFileCloseArchive, BOOL, __stdcall, (HANDLE hArchive), 0x147c0);  //Storm.#252

/// Imported by ['D2Launch.dll', 'Fog.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SFileCloseFile, BOOL, __stdcall, (HANDLE hFile), 0x14900);  //Storm.#253

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SFileDdaBegin, int, __stdcall, (HANDLE hFile, int a2, int a3), 0x14b30);  //Storm.#254

/// Imported by ['D2sound.dll']
D2FUNC_DLL_NP(STORM, SFileDdaBeginEx, int, __stdcall, (HANDLE hFile, signed int a2, unsigned int a3, unsigned __int32 lDistanceToMove, signed int a5, signed int a6, int a7), 0x14b60);  //Storm.#255

/// Imported by ['D2sound.dll']
D2FUNC_DLL_NP(STORM, SFileDdaDestroy, BOOL, __cdecl, (), 0x167f0);  //Storm.#256

/// Imported by ['D2sound.dll']
D2FUNC_DLL_NP(STORM, SFileDdaEnd, BOOL, __stdcall, (HANDLE directsound), 0x16a00);  //Storm.#257

/// Imported by ['D2sound.dll']
D2FUNC_DLL_NP(STORM, SFileDdaGetPos, BOOL, __stdcall, (int a1, int a2, int a3), 0x16b30);  //Storm.#258

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SFileDdaGetVolume, BOOL, __stdcall, (int a1, int a2, int a3), 0x16be0);  //Storm.#259

/// Imported by ['D2sound.dll']
D2FUNC_DLL_NP(STORM, SFileDdaInitialize, BOOL, __stdcall, (HANDLE directsound), 0x16cd0);  //Storm.#260

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SFileDdaSetVolume, BOOL, __stdcall, (HANDLE directsound, signed int bigvolume, signed int volume), 0x16d00);  //Storm.#261

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SFileDestroy, BOOL, __cdecl, (), 0x16dc0);  //Storm.#262

/// Imported by ['Fog.dll']
D2FUNC_DLL_NP(STORM, SFileEnableDirectAccess, BOOL, __stdcall, (HANDLE hFile), 0x17040);  //Storm.#263

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SFileGetFileArchive, BOOL, __stdcall, (HANDLE hFile, HANDLE archive), 0x17220);  //Storm.#264

/// Imported by ['Fog.dll']
D2FUNC_DLL_NP(STORM, SFileGetFileSize, int, __stdcall, (int, LPDWORD lpFileSizeHigh), 0x17300);  //Storm.#265

/// Imported by ['D2Win.dll']
D2FUNC_DLL_NP(STORM, SFileOpenArchive, int, __stdcall, (char *Str, int, int, int), 0x17510);  //Storm.#266

/// Imported by ['Fog.dll']
D2FUNC_DLL_NP(STORM, SFileOpenFile, int, __stdcall, (char *Str, int), 0x17ed0);  //Storm.#267

/// Imported by ['D2Launch.dll', 'D2Direct3D.dll', 'D2Glide.dll', 'D2DDraw.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SFileOpenFileEx, int, __stdcall, (int, char *Str, char, int), 0x17ef0);  //Storm.#268

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SFileReadFile, int, __stdcall, (int, int, int, int, LONG lDistanceToMove), 0x188e0);  //Storm.#269

/// Imported by ['Fog.dll']
D2FUNC_DLL_NP(STORM, SFileSetBasePath, BOOL, __stdcall, (char *path), 0x18fd0);  //Storm.#270

/// Imported by ['Fog.dll']
D2FUNC_DLL_NP(STORM, SFileSetFilePointer, int, __stdcall, (int, LONG lDistanceToMove, int, DWORD dwMoveMethod), 0x190c0);  //Storm.#271

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SFileSetLocale, LCID, __stdcall, (LCID lcLocale), 0x19220);  //Storm.#272

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SFileGetBasePath, BOOL, __stdcall, (char *result, int maxSize), 0x17170);  //Storm.#273

/// Imported by ['Fog.dll']
D2FUNC_DLL_NP(STORM, SFileSetIoErrorMode, BOOL, __stdcall, (int a1, int a2), 0x19200);  //Storm.#274

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SFileGetArchiveName, BOOL, __stdcall, (HANDLE hArchive, char *name, int length), 0x17100);  //Storm.#275

/// Imported by ['D2CMP.dll', 'D2Common.dll', 'D2sound.dll', 'D2Win.dll', 'D2Client.dll', 'D2Lang.dll']
D2FUNC_DLL_NP(STORM, SFileGetFileName, BOOL, __stdcall, (HANDLE hFile, char *buffer, int length), 0x17290);  //Storm.#276

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SFileGetArchiveInfo, int, __stdcall, (int a1, int a2, int a3), 0x17070);  //Storm.#277

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SFileSetPlatform, char, __stdcall, (char a1), 0x19230);  //Storm.#278

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SFileLoadFile, int, __stdcall, (char *Str, int, int, int, LONG lDistanceToMove), 0x17380);  //Storm.#279

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SFileUnloadFile, BOOL, __stdcall, (void *buffer), 0x19240);  //Storm.#280

/// Imported by ['D2Direct3D.dll', 'D2Gdi.dll', 'D2Glide.dll', 'D2DDraw.dll']
D2FUNC_DLL_NP(STORM, SFileLoadFileEx, int, __stdcall, (int, char *Str, int, int, int, char, LONG lDistanceToMove), 0x173e0);  //Storm.#281

/// Imported by ['Fog.dll']
D2FUNC_DLL_NP(STORM, SFilePrioritizeRequest, void, __stdcall, (int a1, int a2), 0x186f0);  //Storm.#282

/// Imported by ['Fog.dll']
D2FUNC_DLL_NP(STORM, SFileCancelRequest, void, __stdcall, (int a1), 0x14280);  //Storm.#283

/// Imported by ['Fog.dll']
D2FUNC_DLL_NP(STORM, SFileSetAsyncBudget, int, __stdcall, (int a1), 0x18fc0);  //Storm.#284

/// Imported by ['Fog.dll']
D2FUNC_DLL_NP(STORM, SFileSetDataChunkSize, void, __stdcall, (int a1), 0x19090);  //Storm.#285

/// Imported by ['Fog.dll']
D2FUNC_DLL_NP(STORM, SFileEnableSeekOptimization, int, __stdcall, (int a1), 0x17060);  //Storm.#286

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SFileReadFileEx, int, __stdcall, (int, int, int, int, LONG lDistanceToMove, int), 0x18910);  //Storm.#287

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SFileFileExists, DWORD, __stdcall, (HANDLE hFile), 0x14790);  //Storm.#288

/// Imported by ['Fog.dll']
D2FUNC_DLL_NP(STORM, SFileFileExistsEx, int, __stdcall, (int, int, int, int, LONG lDistanceToMove, int, int), 0x18940);  //Storm.#289

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SFileReadFileEx2, int, __stdcall, (int, char *Str, int, int, int, char, LONG lDistanceToMove, int), 0x17410);  //Storm.#290

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, StormDestroy, BOOL, __cdecl, (), 0x2b300);  //Storm.#301

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, StormGetInstance, HMODULE, __cdecl, (), 0x2b350);  //Storm.#302

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, StormGetOption, BOOL, __stdcall, (int type, int optval, size_t optlen), 0x2b360);  //Storm.#303

/// Imported by ['Fog.dll']
D2FUNC_DLL_NP(STORM, StormSetOption, BOOL, __stdcall, (int type, int optval, size_t optlen), 0x2b440);  //Storm.#304

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBltGetSCode, BOOL, __stdcall, (unsigned int a1, char *buffer, size_t buffersize, int a4), 0x5430);  //Storm.#312

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBltROP3, BOOL, __stdcall, (int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8), 0x5640);  //Storm.#313

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBltROP3Clipped, BOOL, __stdcall, (int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10), 0x58e0);  //Storm.#314

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBltROP3Tiled, BOOL, __stdcall, (int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10), 0x5a90);  //Storm.#315

/// Imported by ['D2Direct3D.dll', 'D2Gdi.dll', 'D2Glide.dll', 'D2DDraw.dll']
D2FUNC_DLL_NP(STORM, SBmpDecodeImage, BOOL, __stdcall, (int a1, signed int *a2, unsigned int a3, int a4, int a5, int a6, int a7, int a8, int a9), 0x5da0);  //Storm.#321

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBmpLoadImage, int, __stdcall, (char *Str, int, int, int, int, int, int), 0x68d0);  //Storm.#323

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBmpSaveImage, int, __stdcall, (LPCSTR lpFileName, int, LPCVOID lpBuffer, int, int, int), 0x6fb0);  //Storm.#324

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBmpAllocLoadImage, int, __stdcall, (char *Str, int, int, int, int, int, int, int), 0x5be0);  //Storm.#325

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBmpSaveImageEx, int, __stdcall, (LPCSTR lpFileName, int, LPCVOID lpBuffer, int, int, int, int), 0x6fe0);  //Storm.#326

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SCodeCompile, BOOL, __stdcall, (char *directives1, char *directives2, int a3, unsigned int a4, unsigned int flags, char *src), 0xa4c0);  //Storm.#331

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SCodeDelete, BOOL, __stdcall, (int handle), 0xab30);  //Storm.#332

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SCodeExecute, int, __cdecl, (int a1, int a2), 0xac20);  //Storm.#334

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SCodeGetPseudocode, BOOL, __stdcall, (int scodestring, int buffer, size_t buffersize), 0xaf90);  //Storm.#335

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDrawAutoInitialize, int, __stdcall, (HINSTANCE hInst, LPCSTR lpClassName, LPCSTR lpCaption, int, int, int nWidth, int nHeight, int), 0x106d0);  //Storm.#341

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDrawCaptureScreen, BOOL, __stdcall, (char *source), 0x10ef0);  //Storm.#342

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDrawClearSurface, BOOL, __stdcall, (int surfacenumber), 0x110e0);  //Storm.#343

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDrawDestroy, BOOL, __cdecl, (), 0x111a0);  //Storm.#344

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDrawFlipPage, BOOL, __cdecl, (), 0x112a0);  //Storm.#345

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDrawGetFrameWindow, HWND, __stdcall, (int sdraw_framewindow), 0x11320);  //Storm.#346

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDrawGetObjects, BOOL, __stdcall, (int obj1, int obj2, int obj3, int obj4, int obj5, int obj6, int obj7), 0x11340);  //Storm.#347

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDrawGetScreenSize, BOOL, __stdcall, (int width, int height, int depth), 0x113c0);  //Storm.#348

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDrawGetServiceLevel, BOOL, __stdcall, (int a1), 0x11490);  //Storm.#349

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDrawLockSurface, BOOL, __stdcall, (int surfacenumber, RECT *lpDestRect, void **lplpSurface, int *lpPitch, int arg_unused), 0x114f0);  //Storm.#350

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDrawManualInitialize, BOOL, __stdcall, (HWND a1, int obj1, int obj2, int obj3, int obj4, int obj5, int obj6, void *obj7), 0x11690);  //Storm.#351

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDrawMessageBox, int, __stdcall, (LPCSTR lpText, LPCSTR lpCaption, UINT uType), 0x11720);  //Storm.#352

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDrawPostClose, BOOL, __cdecl, (), 0x11840);  //Storm.#353

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDrawRealizePalette, BOOL, __cdecl, (), 0x11860);  //Storm.#354

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDrawSelectGdiSurface, int, __stdcall, (unsigned int a1, int a2), 0x11960);  //Storm.#355

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDrawUnlockSurface, BOOL, __stdcall, (int surfacenumber, void *lpSurface, int zero1, int zero2), 0x11a00);  //Storm.#356

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDrawUpdatePalette, BOOL, __stdcall, (unsigned int firstentry, unsigned int numentries, PALETTEENTRY *pPalEntries, int a4), 0x11b00);  //Storm.#357

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDrawUpdateScreen, BOOL, __stdcall, (int a1), 0x11bc0);  //Storm.#358

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SDrawWaitForVerticalBlank, int, __cdecl, (), 0x11d80);  //Storm.#359

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SEvtDispatch, BOOL, __stdcall, (DWORD messageID, int a2, int a3, int a4), 0x12cc0);  //Storm.#372

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SEvtRegisterHandler, BOOL, __stdcall, (DWORD messageID, unsigned int a2, int event, int a4, int a5), 0x130a0);  //Storm.#373

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SEvtUnregisterHandler, BOOL, __stdcall, (DWORD messageID, int a2, int a3, int a4), 0x133c0);  //Storm.#374

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SEvtUnregisterType, BOOL, __stdcall, (DWORD messageID, int a2), 0x134a0);  //Storm.#375

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SEvtPopState, BOOL, __stdcall, (DWORD messageID, int type), 0x12ed0);  //Storm.#376

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SEvtPushState, BOOL, __stdcall, (DWORD messageID, int type), 0x12f60);  //Storm.#377

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SEvtBreakHandlerChain, BOOL, __stdcall, (int a1), 0x12b00);  //Storm.#378

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SGdiBitBlt, BOOL, __stdcall, (int a1, int a2, int a3, int a4, unsigned int a5, int a6, int a7, unsigned int a8, signed int a9), 0x19420);  //Storm.#381

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SGdiCreateFont, BOOL, __stdcall, (const void *handle, int bits, int width, int height, int filecharwidth, int filecharheight, const void *charsizetable, int a8), 0x19600);  //Storm.#382

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SGdiDeleteObject, BOOL, __stdcall, (int a1), 0x197e0);  //Storm.#383

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SGdiExtTextOut, BOOL, __stdcall, (int a1, int a2, int a3, int a4, unsigned int a8, signed int a6, signed int a7, const char *arg1C, unsigned int arg20), 0x19980);  //Storm.#385

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SGdiImportFont, int, __stdcall, (HGDIOBJ h, int), 0x19fa0);  //Storm.#386

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SGdiLoadFont, int, __stdcall, (char *Str, int, int, int, int, int), 0x1a460);  //Storm.#387

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SGdiRectangle, BOOL, __stdcall, (int a1, int a2, int a3, int a4, int a5, int a6), 0x1a5c0);  //Storm.#388

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SGdiSelectObject, BOOL, __stdcall, (int a1), 0x1a700);  //Storm.#389

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SGdiSetPitch, BOOL, __stdcall, (int pitch), 0x1a740);  //Storm.#390

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SGdiTextOut, BOOL, __stdcall, (int a1, int a2, int a3, unsigned int a8, const char *arg1C, unsigned int arg20), 0x1a870);  //Storm.#391

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SGdi392, BOOL, __stdcall, (int, int, int, int), 0x1a7a0);  //Storm.#392

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, Ordinal393, BOOL, __stdcall, (int, int, int), 0x19f00);  //Storm.#393

/// Imported by ['D2Launch.dll', 'D2OpenGL.dll', 'D2Glide.dll', 'Fog.dll', 'D2CMP.dll', 'D2Win.dll', 'D2Game.dll', 'D2Client.dll', 'D2Lang.dll']
D2FUNC_DLL_NP(STORM, SMemAlloc, void*, __stdcall, (int amount, const char *logfilename, int logline, uint32_t flags), 0x1b130);  //Storm.#401

/// Imported by ['D2Launch.dll', 'D2Direct3D.dll', 'D2OpenGL.dll', 'D2Gdi.dll', 'D2Glide.dll', 'Fog.dll', 'D2CMP.dll', 'D2DDraw.dll', 'D2Win.dll', 'D2Game.dll', 'D2Client.dll', 'D2Lang.dll']
D2FUNC_DLL_NP(STORM, SMemFree, BOOL, __stdcall, (void *location, const char *logfilename, int logline, uint32_t flags), 0x1c2e0);  //Storm.#403

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SMemGetSize, size_t, __stdcall, (void *location, const char *logfilename, int logline), 0x1c4f0);  //Storm.#404

/// Imported by ['D2CMP.dll', 'D2Game.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SMemReAlloc, void*, __stdcall, (void *location, int amount, const char *logfilename, int logline, uint32_t flags), 0x1ce40);  //Storm.#405

/// Imported by ['D2Win.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SMsgDispatchMessage, BOOL, __stdcall, (HWND hWnd, DWORD messageID, WPARAM wParam, LPARAM lParam, int a5, int a6), 0x1d200);  //Storm.#412

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SMsgDoMessageLoop, WPARAM, __stdcall, (void *callback, int followWithStormDestroy), 0x1d310);  //Storm.#413

/// Imported by ['D2Win.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SMsgRegisterCommand, BOOL, __stdcall, (int a1, int a2, int a3), 0x1d4c0);  //Storm.#414

/// Imported by ['D2Win.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SMsgRegisterKeyDown, BOOL, __stdcall, (int a1, int a2, int a3), 0x1d5b0);  //Storm.#415

/// Imported by ['D2Win.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SMsgRegisterKeyUp, BOOL, __stdcall, (int a1, int a2, int a3), 0x1d5d0);  //Storm.#416

/// Imported by ['D2Win.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SMsgRegisterMessage, BOOL, __stdcall, (int a1, int a2, int a3), 0x1d5f0);  //Storm.#417

/// Imported by ['D2Client.dll']
D2FUNC_DLL_NP(STORM, SMsgPopRegisterState, BOOL, __stdcall, (int type), 0x1d440);  //Storm.#418

/// Imported by ['D2Client.dll']
D2FUNC_DLL_NP(STORM, SMsgPushRegisterState, BOOL, __stdcall, (int type), 0x1d480);  //Storm.#419

/// Imported by ['D2Direct3D.dll', 'D2DDraw.dll']
D2FUNC_DLL_NP(STORM, SRegLoadData, int, __stdcall, (int, DWORD Type, int, LPBYTE lpData, int, LPDWORD lpcbData), 0x25840);  //Storm.#421

/// Imported by ['D2Launch.dll', 'D2Direct3D.dll', 'Fog.dll', 'D2DDraw.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SRegLoadString, int, __stdcall, (DWORD Type, DWORD cbData, int, LPSTR, int), 0x25a00);  //Storm.#422

/// Imported by ['D2gfx.dll', 'D2Launch.dll', 'D2Direct3D.dll', 'Fog.dll', 'D2CMP.dll', 'D2Multi.dll', 'D2DDraw.dll', 'D2sound.dll', 'D2Game.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SRegLoadValue, BOOL, __stdcall, (const char *keyname, const char *valuename, int a3, DWORD *value), 0x25ac0);  //Storm.#423

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SRegSaveData, BOOL, __stdcall, (char *keyname, char *valuename, BYTE flags, BYTE *lpData, DWORD cbData), 0x25ba0);  //Storm.#424

/// Imported by ['D2Launch.dll', 'Fog.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SRegSaveString, BOOL, __stdcall, (char *keyname, char *valuename, BYTE flags, char *string), 0x25d10);  //Storm.#425

/// Imported by ['D2Launch.dll', 'D2Multi.dll', 'D2sound.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SRegSaveValue, BOOL, __stdcall, (const char *keyname, const char *valuename, BYTE flags, DWORD result), 0x25d80);  //Storm.#426

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SRegGetBaseKey, BOOL, __stdcall, (char flags, char *buffer, size_t bufferchars), 0x257e0);  //Storm.#427

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SRegDeleteValue, BOOL, __stdcall, (char *keyname, char *valuename, HKEY phkResult), 0x25660);  //Storm.#428

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, STransBlt, BOOL, __stdcall, (int a1, int a2, int a3, int a4, int a5), 0x2b640);  //Storm.#431

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, STransBltUsingMask, BOOL, __stdcall, (int a1, int a2, int a3, int a4, int a5), 0x2b710);  //Storm.#432

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, STransCreateI, BOOL, __stdcall, (int a1, int a2, int a3, signed int a4, int a5, int a6, int a7), 0x2bc70);  //Storm.#433

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, STransDelete, BOOL, __stdcall, (int a1), 0x2bda0);  //Storm.#434

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, STransDuplicate, BOOL, __stdcall, (int handle, int source), 0x2beb0);  //Storm.#436

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, STransIntersectDirtyArray, BOOL, __stdcall, (int handle, int dirtyarraymask, unsigned __int8 dirtyarray, int sourcemask), 0x2bff0);  //Storm.#437

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, STransInvertMask, BOOL, __stdcall, (int handle, int sourcemask), 0x2bf20);  //Storm.#438

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, STransLoadI, int, __stdcall, (char *Str, int, int, int), 0x2c9f0);  //Storm.#439

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, STransSetDirtyArrayInfo, BOOL, __stdcall, (signed int depth, int depth2, int height, int width), 0x2ca40);  //Storm.#440

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, STransUpdateDirtyArray, BOOL, __stdcall, (int a1, char a2, int a3, int a4, int a5, int a6), 0x2cb50);  //Storm.#441

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, STransPointInMask, BOOL, __stdcall, (int, int, int, char *, size_t), 0x2c860);  //Storm.#442

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, STransCombineMasks, BOOL, __stdcall, (int, int, int, int, int, int), 0x2c350);  //Storm.#443

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, STransCreateI2, BOOL, __stdcall, (int, int, int, int, int, int, int), 0x2bd40);  //Storm.#444

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, STransCreateE, BOOL, __stdcall, (int, int, int, int, int, int, int), 0x2b7e0);  //Storm.#445

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, STrans446, BOOL, __stdcall, (int, int, int, int, int, int, int), 0x2bcd0);  //Storm.#446

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, STransLoadE, int, __stdcall, (char *Str, int, int, int), 0x2c900);  //Storm.#447

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SVidDestroy, BOOL, __cdecl, (), 0x2d690);  //Storm.#451

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SVidGetSize, BOOL, __stdcall, (int video, int a2, int a3, int a4), 0x2d750);  //Storm.#452

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SVidInitialize, BOOL, __stdcall, (int arg0), 0x2d820);  //Storm.#453

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SVidPlayBegin, int, __stdcall, (char *Str, int, int, int, int, int, int), 0x2db10);  //Storm.#454

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SVidPlayBeginFromMemory, int, __stdcall, (LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, int, int, int, int, int, int), 0x2e430);  //Storm.#455

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SVidPlayContinue, BOOL, __cdecl, (), 0x2e5c0);  //Storm.#456

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SVidPlayContinueSingle, BOOL, __stdcall, (int video, int a2, int a3), 0x2e600);  //Storm.#457

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SVidPlayEnd, BOOL, __stdcall, (int video), 0x2f170);  //Storm.#458

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SVidSetVolume, BOOL, __stdcall, (int video, int a2, int a3, int type), 0x2f290);  //Storm.#459

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SErrDisplayError, BOOL, __stdcall, (uint32_t errorcode, const char* filename, int32_t linenumber, const char* description, int32_t recoverable, uint32_t exitcode), 0x12020);  //Storm.#461
/// Imported by ['D2Client.dll']
D2FUNC_DLL_NP(STORM, SErrGetErrorStr, int, __stdcall, (DWORD dwMessageId, LPSTR lpBuffer, DWORD nSize), 0x125d0);  //Storm.#462

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SErrGetLastError, DWORD, __cdecl, (), 0x126f0);  //Storm.#463

/// Imported by ['D2Client.dll']
D2FUNC_DLL_NP(STORM, SErrRegisterMessageSource, BOOL, __stdcall, (__int16 a1, int a2, int a3), 0x12800);  //Storm.#464

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SErrSetLastError, void, __stdcall, (DWORD dwErrCode), 0x12940);  //Storm.#465

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SErrReportNamedResourceLeak, int, __stdcall, (char ArgList, int), 0x12850);  //Storm.#466

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SErrReportResourceLeak, int, __stdcall, (char ArgList), 0x12840);  //Storm.#467

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SErrSuppressErrors, BOOL, __stdcall, (BOOL suppressErrors), 0x12960);  //Storm.#468

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SErrRegisterHandler, int, __stdcall, (int), 0x12740);  //Storm.#469

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SErrUnregisterHandler, void, __stdcall, (int), 0x12970);  //Storm.#470

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SCmdGetBool, BOOL, __stdcall, (int a1), 0x80a0);  //Storm.#472

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SCmdGetNum, int, __stdcall, (int a1), 0x80c0);  //Storm.#473

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SCmdGetString, BOOL, __stdcall, (int a1, char *dest, size_t size), 0x8100);  //Storm.#474

/// Imported by ['D2Client.dll']
D2FUNC_DLL_NP(STORM, SCmdProcess, BOOL, __stdcall, (const char **string, int a2, int a3, int a4), 0x8180);  //Storm.#475

/// Imported by ['D2Client.dll']
D2FUNC_DLL_NP(STORM, SCmdRegisterArgList, BOOL, __stdcall, (int, int), 0x8b30);  //Storm.#476

/// Imported by ['D2Client.dll']
D2FUNC_DLL_NP(STORM, SCmdRegisterArgument, BOOL, __stdcall, (int a1, int a2, char *str, int a4, int a5, int a6, int a7, int a8), 0x8b90);  //Storm.#477

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SCmdStringExists, int, __stdcall, (int), 0x7f90);  //Storm.#478

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SCmdProcessCommandLine, BOOL, __stdcall, (int, int), 0x8b10);  //Storm.#479

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SMemFindNextBlock, BOOL, __stdcall, (int, int, int, int), 0x1be80);  //Storm.#481

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SMemFindNextHeap, BOOL, __stdcall, (int prevheap, int details, int nextheap), 0x1c130);  //Storm.#482

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SMemGetHeapByCaller, int, __stdcall, (int, int), 0x1c3e0);  //Storm.#483

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SMemGetHeapByPtr, int, __stdcall, (int), 0x1c480);  //Storm.#484

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SMemHeapAlloc, int, __stdcall, (int, int, int), 0x1c570);  //Storm.#485

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SMemHeapCreate, int, __stdcall, (int, SIZE_T, int), 0x1c640);  //Storm.#486

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SMemHeapDestroy, BOOL, __stdcall, (int), 0x1c780);  //Storm.#487

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SMemHeapFree, BOOL, __stdcall, (int a1, int a2, void *a3), 0x1c800);  //Storm.#488

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SMemHeapRealloc, int, __stdcall, (int a1, int a2, void *a3, int a4), 0x1c8c0);  //Storm.#489

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SMemHeapSize, int, __stdcall, (int a1, int a2, void *a3), 0x1cd50);  //Storm.#490

/// Imported by ['D2Launch.dll', 'Fog.dll', 'D2Game.dll']
D2FUNC_DLL_NP(STORM, SMemCopy, int, __stdcall, (void *dest, const void *source, size_t size), 0x1cf80);  //Storm.#491

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SMemFill, int, __stdcall, (void *location, size_t length, char fillWith), 0x1cfb0);  //Storm.#492

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SMemMove, int, __stdcall, (void *Dst, void *Src, size_t Size), 0x1cff0);  //Storm.#493

/// Imported by ['D2Launch.dll', 'Fog.dll', 'D2Multi.dll']
D2FUNC_DLL_NP(STORM, SMemZero, int, __stdcall, (void *location, size_t length), 0x1d010);  //Storm.#494

/// Imported by ['Fog.dll']
D2FUNC_DLL_NP(STORM, SMemCmp, int, __stdcall, (void *location1, void *location2, size_t size), 0x1cf60);  //Storm.#495

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SMem496, int, __stdcall, (int, char), 0x1cf10);  //Storm.#496

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SMemDumpState, BOOL, __stdcall, (int, int), 0x1bd60);  //Storm.#497

/// Imported by ['D2Launch.dll', 'Fog.dll', 'D2MCPClient.dll', 'D2Common.dll', 'D2Multi.dll', 'D2Win.dll', 'D2Game.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SStrCopy, size_t, __stdcall, (char *dest, const char *source, size_t destsize), 0x2a120);  //Storm.#501
/// Imported by ['D2Launch.dll', 'Fog.dll', 'D2CMP.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SStrHash, DWORD, __stdcall, (const char *string, unsigned __int32 flags, unsigned __int32 Seed), 0x2ad40);  //Storm.#502

/// Imported by ['D2Launch.dll', 'Fog.dll', 'D2Win.dll']
D2FUNC_DLL_NP(STORM, SStrNCat, int, __stdcall, (char *base, char *append, int max_length), 0x2a3c0);  //Storm.#503

/// Imported by ['D2Client.dll']
D2FUNC_DLL_NP(STORM, SStrTokenize, void, __stdcall, (const char** string, char* buffer, size_t bufferchars, const char* delimiter, BOOL* quoted), 0x2abe0); //Storm.#504

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SStrSearch, char*, __stdcall, (const char *string, char c, BOOL bReverse), 0x2a060);  //Storm.#505

/// Imported by ['D2Launch.dll', 'Fog.dll', 'D2MCPClient.dll', 'D2Multi.dll', 'D2Game.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SStrLen, size_t, __stdcall, (const char *string), 0x2a330);  //Storm.#506

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SStrDup, void*, __stdcall, (int a1), 0x2a260);  //Storm.#507

/// Imported by ['D2Launch.dll', 'Fog.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SStrCmp, int32_t, __stdcall, (const char *Str1, const char *Str2, size_t MaxCount), 0x2a0a0);  //Storm.#508

/// Imported by ['D2Launch.dll', 'Fog.dll', 'D2CMP.dll', 'D2Common.dll', 'D2Game.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SStrCmpI, int32_t, __stdcall, (const char *Str1, const char *Str, size_t MaxCount), 0x2a0e0);  //Storm.#509

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SStrUpper, char*, __stdcall, (char *Str), 0x2b240);  //Storm.#510

/// Imported by ['D2Win.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SMsgBreakHandlerChain, BOOL, __stdcall, (int a1), 0x1d110);  //Storm.#511

/// Imported by ['D2Win.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SMsgUnregisterCommand, BOOL, __stdcall, (int a1, int a2, int a3), 0x1d630);  //Storm.#512

/// Imported by ['D2Win.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SMsgUnregisterKeyDown, BOOL, __stdcall, (int a1, int a2, int a3), 0x1d680);  //Storm.#513

/// Imported by ['D2Win.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SMsgUnregisterKeyUp, BOOL, __stdcall, (int a1, int a2, int a3), 0x1d6a0);  //Storm.#514

/// Imported by ['D2Win.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SMsgUnregisterMessage, BOOL, __stdcall, (int a1, int a2, int a3), 0x1d6c0);  //Storm.#515

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SMsgGetDispatcher, int, __stdcall, (int a1), 0x1d3a0);  //Storm.#516

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SMsgSetDefaultWindow, BOOL, __stdcall, (int a1), 0x1d610);  //Storm.#517

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SMsgGetDefaultWindow, HWND, __cdecl, (), 0x1d390);  //Storm.#518

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SRgn521, int, __stdcall, (int a1), 0x26060);  //Storm.#521

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SRgn523, void, __stdcall, (int handle, int a2, int a3, int a4), 0x269f0);  //Storm.#523

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SRgnCreateRegion, void, __stdcall, (int handle, int a2), 0x26a50);  //Storm.#524

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SRgnDeleteRegion, void, __stdcall, (int handle), 0x26b50);  //Storm.#525

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SRgn527, void, __stdcall, (int handle, int a2, int a3), 0x26d00);  //Storm.#527

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SRgn528i, void, __stdcall, (int handle, int a2, int a3, int a4), 0x279f0);  //Storm.#528

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SRgn529i, void, __stdcall, (int handle, int a2, int a3), 0x27b10);  //Storm.#529

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SRgn530i, void, __stdcall, (int handle, int a2), 0x27000);  //Storm.#530

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SRgn531i, BOOL, __stdcall, (int handle, int a2, int a3), 0x27c60);  //Storm.#531

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SRgn532i, BOOL, __stdcall, (int handle, int a2), 0x27d60);  //Storm.#532

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SRgn533i, void, __stdcall, (int handle, int a2, int a3), 0x27e50);  //Storm.#533

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SRgn534, void, __stdcall, (int handle, int a2, int a3, int a4), 0x26130);  //Storm.#534

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SRgn535f, void, __stdcall, (int handle, float a2, float a3, float a4), 0x27070);  //Storm.#535

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SRgn536f, void, __stdcall, (int handle, float a2, float a3), 0x27a50);  //Storm.#536

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SRgn537f, void, __stdcall, (int handle, float a2), 0x26ed0);  //Storm.#537

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SRgn538f, int, __stdcall, (int, float, float), 0x27b80);  //Storm.#538

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SRgn539f, BOOL, __stdcall, (int handle, float a2), 0x27c80);  //Storm.#539

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SRgn540f, int, __stdcall, (int, float, float), 0x27db0);  //Storm.#540

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SLogClose, void, __stdcall, (DWORD NumberOfBytesWritten), 0x1a8e0);  //Storm.#541

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SLogCreate, int, __stdcall, (LPCSTR lpFileName, int, int), 0x1aa60);  //Storm.#542

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SLog543, void, __stdcall, (int a1, int a2), 0x1b070);  //Storm.#543

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SLogDump, void, __stdcall, (int a1, int a2, DWORD NumberOfBytesWritten), 0x1aba0);  //Storm.#544

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SLogFlush, void, __stdcall, (DWORD NumberOfBytesWritten), 0x1aef0);  //Storm.#545

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SLogFlushAll, void, __cdecl, (), 0x1af20);  //Storm.#546

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SLogPend, int, __cdecl, (int, char *Format, char Args), 0x1af90);  //Storm.#547

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SLogWrite, int, __cdecl, (int, char *Format, char Args), 0x1b0a0);  //Storm.#548

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SLog549, void, __stdcall, (int a1, int a2), 0x1b000);  //Storm.#549

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SLogCriticalLog, void, __stdcall, (HANDLE handle, int a2), 0x1b030);  //Storm.#550

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SCompCompress, BOOL, __stdcall, (void *a1, int a2, unsigned int a3, signed int amount, int a5, char a6, int a7), 0xca50);  //Storm.#551

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SCompDecompress, BOOL, __stdcall, (void *a1, int *a2, const void *a3, unsigned int a4), 0xcc10);  //Storm.#552

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SErrCheckDebugSymbolLibrary, int, __cdecl, (char *Format, char Args), 0x11f50);  //Storm.#561

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SErrDisplayErrorFmt, int, __cdecl, (DWORD dwMessageId, const char* filename, int32_t linenumber, int32_t recoverable, UINT uExitCode, char* Format, ...), 0x12570); //Storm.#562

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SErrIsDisplayingError, int, __cdecl, (), 0x12700);  //Storm.#563

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SErrPrepareAppFatal, int, __stdcall, (int a1, int a2), 0x12710);  //Storm.#564

/// Imported by ['D2Launch.dll', 'Fog.dll', 'D2Multi.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SStrChr, const char*, __stdcall, (const char *string, char c), 0x29fe0);  //Storm.#571

/// Imported by ['Fog.dll']
D2FUNC_DLL_NP(STORM, SStrChrR, const char*, __stdcall, (const char *string, char c), 0x2a020);  //Storm.#572

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SStrToDouble, double, __stdcall, (const char *string), 0x2a650);  //Storm.#573

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SStrToFloat, float, __stdcall, (const char *string), 0x2a830);  //Storm.#574

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SStrToInt, int32_t, __stdcall, (const char *string), 0x2aa10);  //Storm.#575

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SStrToUnsigned, unsigned int, __stdcall, (const char *string), 0x2ab90);  //Storm.#576

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SStrToInt64, __int64, __stdcall, (const char *string), 0x2aa80);  //Storm.#577

/// Imported by ['D2Launch.dll', 'Fog.dll', 'D2Common.dll', 'D2Multi.dll', 'D2Client.dll']
D2FUNC_DLL_NP(STORM, SStrPrintf, int, __cdecl, (char* DstBuf, size_t MaxCount, char* Format, ...), 0x2a570); //Storm.#578

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SStrLower, int, __stdcall, (char *Str), 0x2b250);  //Storm.#579

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SStrHash64, int, __stdcall, (int a1, int a2, int a3, int a4), 0x2ae20);  //Storm.#580

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SStrVPrintf, int, __cdecl, (char *DstBuf, size_t MaxCount, char *Format, va_list ArgList), 0x2a610);  //Storm.#581

/// Imported by ['D2Launch.dll', 'D2Multi.dll']
D2FUNC_DLL_NP(STORM, SDrawSetClientRect, void, __stdcall, (LPRECT lpRect), 0x2b1d0);  //Storm.#582

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigAdd, int, __stdcall, (void *buffer, const void *buffer2, const void *buffer3), 0x10b0);  //Storm.#601

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigAnd, int, __stdcall, (void *buffer, const void *buffer2, const void *buffer3), 0x1230);  //Storm.#602

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigCompare, int, __stdcall, (void *buffer, void *buffer2), 0x1370);  //Storm.#603

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigCopy, int, __stdcall, (void *buffer, void *buffer2), 0x1440);  //Storm.#604

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigDec, int, __stdcall, (void *buffer, const void *buffer2), 0x14c0);  //Storm.#605

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigDel, int, __stdcall, (void *buffer), 0x1620);  //Storm.#606

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigDiv, int, __stdcall, (int a1, const void *buffer1, const void *buffer2), 0x16a0);  //Storm.#607

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigFindPrime, int, __stdcall, (int a1, void *a2, unsigned int a3, const void *a4), 0x2370);  //Storm.#608

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigFromBinary, int, __stdcall, (void *buffer, const void *str, size_t size), 0x3510);  //Storm.#609

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigFromStr, int, __stdcall, (void *buffer, int a2), 0x3670);  //Storm.#610

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigFromStream, int, __stdcall, (void *buffer, int a2, int a3, int a4), 0x36c0);  //Storm.#611

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigFromUnsigned, int, __stdcall, (void *buffer, unsigned int value), 0x37a0);  //Storm.#612

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigGcd, int, __stdcall, (void *buffer, int a2, int a3), 0x37b0);  //Storm.#613

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigInc, int, __stdcall, (void *buffer, int a2), 0x37d0);  //Storm.#614

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigInvMod, int, __stdcall, (void *buffer, int a2, int a3), 0x37f0);  //Storm.#615

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigIsEven, int, __stdcall, (void *buffer), 0x39a0);  //Storm.#616

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigIsOdd, int, __stdcall, (void *buffer), 0x39b0);  //Storm.#617

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigIsOne, int, __stdcall, (void *buffer), 0x3a40);  //Storm.#618

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigIsPrime, int, __stdcall, (void *buffer), 0x3a50);  //Storm.#619

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigIsZero, int, __stdcall, (void *buffer), 0x3a60);  //Storm.#620

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigMod, int, __stdcall, (void *buffer, int a2, int a3), 0x3a70);  //Storm.#621

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigMul, int, __stdcall, (void *buffer, int a2, int a3), 0x3ab0);  //Storm.#622

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigMulMod, int, __stdcall, (void *buffer, int a2, int a3, int a4), 0x3ad0);  //Storm.#623

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigNew, int, __stdcall, (void **buffer), 0x3af0);  //Storm.#624

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigNot, int, __stdcall, (void *buffer, int a2), 0x3b70);  //Storm.#625

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigOr, int, __stdcall, (void *buffer, int a2, int a3), 0x3c70);  //Storm.#626

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigPow, int, __stdcall, (void *buffer, int a2, int a3), 0x3db0);  //Storm.#627

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigPowMod, int, __stdcall, (void *buffer, int a2, int a3, int a4), 0x3f10);  //Storm.#628

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigRand, int, __stdcall, (void *buffer, int a2, int a3), 0x3f30);  //Storm.#629

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigSet2Exp, int, __stdcall, (void *buffer, int a2), 0x41e0);  //Storm.#630

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigSetOne, int, __stdcall, (void *buffer), 0x41f0);  //Storm.#631

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigSetZero, int, __stdcall, (void *buffer), 0x4200);  //Storm.#632

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigShl, int, __stdcall, (void *buffer, int a2, int a3), 0x4210);  //Storm.#633

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigShr, int, __stdcall, (void *buffer, int a2, int a3), 0x4230);  //Storm.#634

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigSquare, int, __stdcall, (void *buffer, int a2), 0x4250);  //Storm.#635

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigSub, int, __stdcall, (void *buffer, int a2, int a3), 0x4270);  //Storm.#636

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigToBinaryArray, int, __stdcall, (void *buffer, int a2, int a3), 0x4290);  //Storm.#637

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigToBinaryBuffer, int, __stdcall, (void *buffer, int a2, int a3, int a4), 0x44f0);  //Storm.#638

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigToBinaryPtr, int, __stdcall, (void *buffer, int a2, int a3), 0x4540);  //Storm.#639

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigToStrArray, int, __stdcall, (void *buffer, int a2, int a3), 0x4580);  //Storm.#640

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigToStrBuffer, int, __stdcall, (void *buffer, char *dst, int count), 0x4810);  //Storm.#641

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigToStrPtr, int, __stdcall, (void *buffer, int a2), 0x4840);  //Storm.#642

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigToStreamArray, int, __stdcall, (void *buffer, int a2, int a3), 0x4870);  //Storm.#643

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigToStreamBuffer, int, __stdcall, (void *buffer, int a2, int a3, int a4), 0x4a70);  //Storm.#644

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigToStreamPtr, int, __stdcall, (void *buffer, int a2, int a3), 0x4ac0);  //Storm.#645

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigToUnsigned, int, __stdcall, (void *buffer, int a2), 0x4af0);  //Storm.#646

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SBigXor, int, __stdcall, (void *buffer, int a2, int a3), 0x4b20);  //Storm.#647

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SUniConvertUTF16to8Len, int, __stdcall, (int a1, int a2, int a3), 0x2cc20);  //Storm.#901

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SUniConvertUTF16to8, int, __stdcall, (int a1, int a2, int a3, int a4, int a5, int a6), 0x2cd20);  //Storm.#902

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SUniConvertUTF8to16Len, int, __stdcall, (int a1, int a2, int a3), 0x2cf00);  //Storm.#903

/// Imported by ['D2Lang.dll']
D2FUNC_DLL_NP(STORM, SUniConvertUTF8to16, int, __stdcall, (int a1, int a2, int a3, int a4, int a5, int a6), 0x2d000);  //Storm.#904

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SUniS905, int, __stdcall, (int a1, int a2), 0x2d1a0);  //Storm.#905

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SUniS906, int, __stdcall, (int a1, int a2), 0x2d2a0);  //Storm.#906

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SUniFindAfterUTF8Chr, int, __stdcall, (int a1, int a2), 0x2d3a0);  //Storm.#907

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SUniFindUTF8ChrStart, int, __stdcall, (int a1, int a2), 0x2d3f0);  //Storm.#908

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SUniConvertUTF16To909, int, __stdcall, (unsigned __int32 a1, char *a2, unsigned __int16 *a3), 0x2d410);  //Storm.#909

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SUniConvertUTF16To910, int, __stdcall, (unsigned __int32 a1, char *a2, unsigned __int16 *a3), 0x2d4c0);  //Storm.#910

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SUniConvertUTF16To911, int, __stdcall, (unsigned __int32 a1, char *a2, unsigned __int16 *a3), 0x2d4e0);  //Storm.#911

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SUniConvert912, int, __stdcall, (int a1, int a2, int a3), 0x2d500);  //Storm.#912

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SUniConvert913, int, __stdcall, (int a1, int a2, int a3), 0x2d570);  //Storm.#913

/// Not imported by any .dll
D2FUNC_DLL_NP(STORM, SUniConvert914, int, __stdcall, (int a1, int a2, int a3), 0x2d590);  //Storm.#914

