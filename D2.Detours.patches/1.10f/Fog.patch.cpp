#include <DetoursPatch.h>
#include <stdlib.h>
#include <Calc.h>

//#define DISABLE_ALL_PATCHES
//#define REPLACE_FOG_ALLOCS_BY_MALLOC

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wmicrosoft-cast"
#endif

extern "C" {
    __declspec(dllexport)
    constexpr int __cdecl GetBaseOrdinal() { return 10'000; }
    __declspec(dllexport)
    constexpr int __cdecl GetLastOrdinal() { return 10'267; }
    constexpr int GetOrdinalCount() { return GetLastOrdinal() - GetBaseOrdinal() + 1; }
}

// We are using the following letters:
// /*C*/ to mean that the function has been "Checked" and works
// /*B*/ to mean wean know it is "Broken" and needs fixing.
//`     ` (spaces) to mean that it is "Unknown" wether it works or not

static PatchAction patchActions[GetOrdinalCount()] = {
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_SOCKET_CloseSocket @10000
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_SOCKET_WSAStartup @10001
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_SOCKET_WSACleanup @10002
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_SOCKET_Connect @10003
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_SOCKET_ConnectEx @10004
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_SOCKET_Destroy @10005
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_SOCKET_Receive @10006
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_SOCKET_Send @10007
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_SOCKET_Select @10008
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_SOCKET_SendNoLock @10009
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_SOCKET_Lock @10010
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_SOCKET_Unlock @10011
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_SOCKET_GetPeerName @10012
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_GetHostIPAddress @10013
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_inet_ntoa @10014
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_ResolveHostIPAddress @10015
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_SOCKET_GetError @10016
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_SOCKET_SetBlockingMode @10017
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_csprintf @10018
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_InitErrorMgr @10019
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10020 @10020
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10021 @10021
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_GetSystemInfo @10022
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_DisplayAssert @10023
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_DisplayHalt @10024
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_DisplayWarning @10025
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_DisplayError @10026
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10027 @10027
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10028 @10028
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_Trace @10029
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10030 @10030
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10031 @10031
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10032 @10032
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10033 @10033
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10034 @10034
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10035 @10035
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10036 @10036
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10037 @10037
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10038 @10038
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10039 @10039
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10040 @10040
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10041 @10041
#ifdef REPLACE_FOG_ALLOCS_BY_MALLOC
#define ALLOC_ORDINAL_PATCH_ACTION PatchAction::Ignore
#else
#define ALLOC_ORDINAL_PATCH_ACTION PatchAction::FunctionReplacePatchByOriginal
#endif
    ALLOC_ORDINAL_PATCH_ACTION,                        //   FOG_Alloc @10042
    ALLOC_ORDINAL_PATCH_ACTION,                        //   FOG_Free @10043
    ALLOC_ORDINAL_PATCH_ACTION,                        //   FOG_Realloc @10044
    ALLOC_ORDINAL_PATCH_ACTION,                        //   FOG_AllocPool @10045
    ALLOC_ORDINAL_PATCH_ACTION,                        //   FOG_FreePool @10046
    ALLOC_ORDINAL_PATCH_ACTION,                        //   FOG_ReallocPool @10047
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10048 @10048
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10049 @10049
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10050_EnterCriticalSection @10050
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10051 @10051
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_GetMemoryUsage @10052
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10053 @10053
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10054 @10054
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10055_GetSyncTime @10055
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10056 @10056
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10057 @10057
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10058 @10058
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10059 @10059
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10060 @10060
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10061 @10061
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10062 @10062
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10063 @10063
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10064 @10064
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10065 @10065
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10066 @10066
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10067 @10067
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10068 @10068
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10069 @10069
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10070 @10070
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10071 @10071
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10072 @10072
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10073 @10073
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10074 @10074
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10075 @10075
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10076 @10076
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10077 @10077
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10078 @10078
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10079 @10079
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10080 @10080
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10081 @10081
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10082 @10082
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10083_Cos_LUT @10083
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10084_Sin_LUT @10084
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_Decode14BitsFromString @10085
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_Encode14BitsToString @10086
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_Decode32BitsFromString @10087
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_Encode32BitsToString @10088
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_AsyncDataInitialize @10089
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_AsyncDataDestroy @10090
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_AsyncDataLoadFileEx @10091
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_AsyncDataTestLoaded @10092
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_AsyncDataWaitAndGetBuffer @10093
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_AsyncDataGetBuffer @10094
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_AsyncDataGetBufferSize @10095
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_AsyncDataSetResults @10096
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_AsyncDataFree @10097
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10098 @10098 // NOOP Same as FOG_10098
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_AsyncDataSetPriority @10099
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_AsyncDataHandlePriorityChanges @10100
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_MPQSetConfig @10101
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_FOpenFile @10102
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_FCloseFile @10103
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_FReadFile @10104
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_FGetFileSize @10105
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_FSetFilePointer @10106
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10107 @10107
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10108 @10108
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10109 @10109
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10110 @10110
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10111 @10111
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10112 @10112
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10113 @10113
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10114 @10114
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_GetSavePath @10115
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10116 @10116
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10117 @10117
    PatchAction::FunctionReplacePatchByOriginal,       //   BITMANIP_SetBitState        @10118
    PatchAction::FunctionReplacePatchByOriginal,       //   BITMANIP_GetBitState        @10119
    PatchAction::FunctionReplacePatchByOriginal,       //   BITMANIP_MaskBitstate       @10120
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10121 @10121
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10122 @10122
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10123 @10123
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10124 @10124
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10125 @10125
    PatchAction::FunctionReplacePatchByOriginal,       //   BITMANIP_Initialize         @10126
    PatchAction::FunctionReplacePatchByOriginal,       //   BITMANIP_GetSize            @10127
    PatchAction::FunctionReplacePatchByOriginal,       //   BITMANIP_Write              @10128
    PatchAction::FunctionReplacePatchByOriginal,       //   BITMANIP_ReadSigned         @10129
    PatchAction::FunctionReplacePatchByOriginal,       //   BITMANIP_Read               @10130
    PatchAction::FunctionReplacePatchByOriginal,       //   BITMANIP_GoToNextByte       @10131
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10132 @10132
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10133 @10133
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10134 @10134
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10135 @10135
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10136 @10136
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_ComputeStringCRC16 @10137
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10138 @10138
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10139 @10139
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10140 @10140
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10141 @10141
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_CreateNewPoolSystem @10142
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_DestroyMemoryPoolSystem @10143
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10144 @10144
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10145 @10145
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10146 @10146
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10147 @10147
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10148 @10148
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_InitializeServer @10149
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10150 @10150
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_SetMaxClientsPerGame @10151
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10152 @10152
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10153 @10153
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_WaitForSingleObject @10154
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10155 @10155
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10156 @10156
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10157 @10157
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10158 @10158
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10159 @10159
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10160 @10160
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10161 @10161
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10162 @10162
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10163 @10163
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10164 @10164
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10165 @10165
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10166 @10166
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10167 @10167
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10168 @10168 
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10169 @10169 
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10170 @10170
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10171 @10171
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10172 @10172
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10173 @10173
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10174 @10174
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10175 @10175
    PatchAction::FunctionReplaceOriginalByPatch,       //   FOG_IsWindowsNT @10176
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10177 @10177
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_SetHackListEnabled @10178
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10179 @10179
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10180 @10180
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10181 @10181
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10182_Return @10182
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10183_Return @10183
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10184 @10184
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10185 @10185
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10186 @10186
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10187 @10187
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10188 @10188
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10189 @10189
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10190 @10190
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10191 @10191
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10192 @10192
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10193 @10193
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10194 @10194
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10195 @10195
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10196 @10196
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10197 @10197
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10198 @10198
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10199 @10199
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10200 @10200
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10201 @10201
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10202 @10202
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10203 @10203
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10204 @10204
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10205 @10205
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10206 @10206
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10207 @10207
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_CreateBinFile @10208
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_FreeBinFile @10209
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_GetRecordCountFromBinFile @10210
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_AllocLinker @10211
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_FreeLinker @10212
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_GetLinkIndex @10213
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_GetStringFromLinkIndex @10214
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10215 @10215
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10216_AddRecordToLinkingTable @10216
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_GetRowFromTxt @10217
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10218 @10218
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10219 @10219
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10220 @10220
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10221 @10221
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10222 @10222
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10223 @10223
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10224 @10224
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10225 @10225
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10226 @10226
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_IsExpansion @10227
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10228 @10228
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_ComputeChecksum @10229
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_PopCount @10230
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10231 @10231
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10232 @10232
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10233 @10233
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10234 @10234
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10235 @10235
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10236 @10236
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10237 @10237
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10238 @10238
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10239 @10239
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10240 @10240
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10241 @10241
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10242 @10242
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10243 @10243
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10244 @10244
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10245 @10245
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10246 @10246
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10247 @10247
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10248 @10248
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10249 @10249
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10250 @10250
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10251 @10251
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_LeadingZeroesCount @10252
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DATATBLS_CalcEvaluateExpression @10253
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DATATBLS_CompileExpression @10254
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10255 @10255
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10262 @10262
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10263 @10263
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10256 @10256 
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10257 @10257 
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10258 @10258 
    PatchAction::FunctionReplacePatchByOriginal,       //   gdwBitMasks                 @10259
    PatchAction::FunctionReplacePatchByOriginal,       //   gdwInvBitMasks              @10260
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10261 @10261 
    PatchAction::FunctionReplacePatchByOriginal,       //   FOG_10264 @10264 

};

extern "C" {

__declspec(dllexport)
PatchAction __cdecl GetPatchAction(int ordinal)
{
#ifdef DISABLE_ALL_PATCHES
    return PatchAction::Ignore;
#else
    if (ordinal < GetBaseOrdinal() || ordinal > GetLastOrdinal())
        return PatchAction::FunctionReplacePatchByOriginal;
    
    static_assert(GetOrdinalCount() == (sizeof(patchActions) / sizeof(*patchActions)), "Make sure we have the right number of ordinal patch entries");
    return ::patchActions[ordinal - GetBaseOrdinal()];
#endif
}

static const int FogImageBase = 0x6FF50000;

#ifdef REPLACE_FOG_ALLOCS_BY_MALLOC
void* __fastcall FOG_Debug_AllocPool_Impl(void* pMemPool, int nSize, const char* szFile, int nLine)
{
    return malloc(nSize > 0 ? nSize : 1);
}

void __fastcall FOG_Debug_FreePool_Impl(void* pMemPool, void* pFree, const char* szFile, int nLine)
{
    free(pFree);
}

void* __fastcall FOG_Debug_ReallocPoolImpl(void* pMemPool, void* pMemory, int nSize, const char* szFile, int nLine)
{
    return realloc(pMemory, nSize > 0 ? nSize : 1);
}



void* __fastcall FOG_Debug_Alloc(int nSize, const char* szFile, int nLine, int n0)
{
    return FOG_Debug_AllocPool_Impl(nullptr, nSize, szFile, nLine);
}
void __fastcall FOG_Debug_Free(void* pFree, const char* szFile, int nLine, int n0)
{
    FOG_Debug_FreePool_Impl(nullptr, pFree, szFile, nLine);
}
void* __fastcall FOG_Debug_Realloc(void* pMemory, int nSize, const char* szFile, int nLine, int n0)
{
    return FOG_Debug_ReallocPoolImpl(nullptr, pMemory, nSize, szFile, nLine);
}
void* __fastcall FOG_Debug_AllocPool(void* pMemPool, int nSize, const char* szFile, int nLine, int n0)
{
    return FOG_Debug_AllocPool_Impl(pMemPool, nSize, szFile, nLine);
}
void __fastcall FOG_Debug_FreePool(void* pMemPool, void* pFree, const char* szFile, int nLine, int n0)
{
    FOG_Debug_FreePool_Impl(pMemPool, pFree, szFile, nLine);
}
void* __fastcall FOG_Debug_ReallocPool(void* pMemPool, void* pMemory, int nSize, const char* szFile, int nLine, int n0)
{
    return FOG_Debug_ReallocPoolImpl(pMemPool, pMemory, nSize, szFile, nLine);
}
#endif

static ExtraPatchAction extraPatchActions[] = {    
#ifdef REPLACE_FOG_ALLOCS_BY_MALLOC
    { 0x6FF58F50 - FogImageBase, &FOG_Debug_Alloc, PatchAction::FunctionReplaceOriginalByPatch },
    { 0x6FF58F90 - FogImageBase, &FOG_Debug_Free, PatchAction::FunctionReplaceOriginalByPatch },
    { 0x6FF58FB0 - FogImageBase, &FOG_Debug_Realloc, PatchAction::FunctionReplaceOriginalByPatch },
    { 0x6FF58FF0 - FogImageBase, &FOG_Debug_AllocPool, PatchAction::FunctionReplaceOriginalByPatch },
    { 0x6FF59030 - FogImageBase, &FOG_Debug_FreePool, PatchAction::FunctionReplaceOriginalByPatch },
    { 0x6FF59060 - FogImageBase, &FOG_Debug_ReallocPool, PatchAction::FunctionReplaceOriginalByPatch },
    { 0x6FF59310 - FogImageBase, &FOG_Debug_AllocPool_Impl, PatchAction::FunctionReplaceOriginalByPatch },
    { 0x6FF599C0 - FogImageBase, &FOG_Debug_FreePool_Impl, PatchAction::FunctionReplaceOriginalByPatch },
    { 0x6FF59BE0 - FogImageBase, &FOG_Debug_ReallocPoolImpl, PatchAction::FunctionReplaceOriginalByPatch },
#endif
    { 0, 0, PatchAction::Ignore}, // Here because we need at least one element in the array
};

__declspec(dllexport)
constexpr int __cdecl GetExtraPatchActionsCount() { 

#ifdef DISABLE_ALL_PATCHES
    return 0;
#else
    return sizeof(extraPatchActions) / sizeof(ExtraPatchAction); 
#endif
}

__declspec(dllexport)
ExtraPatchAction* __cdecl GetExtraPatchAction(int index)
{
    return &extraPatchActions[index];
}

}

#include <type_traits>
static_assert(std::is_same<decltype(GetBaseOrdinal)*, GetIntegerFunctionType>::value, "Ensure calling convention doesn't change");
static_assert(std::is_same<decltype(GetLastOrdinal)*, GetIntegerFunctionType>::value, "Ensure calling convention doesn't change");
static_assert(std::is_same<decltype(GetPatchAction)*, GetPatchActionType>::value, "Ensure calling convention doesn't change");


static_assert(std::is_same<decltype(GetExtraPatchActionsCount)*, GetIntegerFunctionType>::value, "Ensure calling convention doesn't change");
static_assert(std::is_same<decltype(GetExtraPatchAction)*, GetExtraPatchActionType>::value, "Ensure calling convention doesn't change");
