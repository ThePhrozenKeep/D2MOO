#include <DetoursPatch.h>

#include <D2DataTbls.h>
#include <Drlg/D2DrlgPreset.h>
#include <Drlg/D2DrlgOutRoom.h>
#include <Drlg/D2DrlgRoomTile.h>
#include <Drlg/D2DrlgActivate.h>
#include <Drlg/D2DrlgOutdoors.h>
#include <Drlg/D2DrlgTileSub.h>
#include <Drlg/D2DrlgOutPlace.h>

//#define DISABLE_ALL_PATCHES

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wmicrosoft-cast"
#endif

extern "C" {
    constexpr int __cdecl GetBaseOrdinal() { return 10'000; }
    constexpr int __cdecl GetLastOrdinal() { return 11'307; }
    constexpr int GetOrdinalCount() { return GetLastOrdinal() - GetBaseOrdinal() + 1; }
}

// We are using the following letters:
// /*C*/ to mean that the function has been "Checked" and works
// /*B*/ to mean wean know it is "Broken" and needs fixing.
//`     ` (spaces) to mean that it is "Unknown" wether it works or not
static PatchAction patchActions[GetOrdinalCount()] = {
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DRLG_GetLevelTypeFromLevelId                                        @10000
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DRLG_GetActNoFromLevelId                                            @10001
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DRLG_GetLOSDrawFromLevelId                                          @10002
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DRLGACTIVATE_Update                                                 @10003
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DRLG_GetSaveMonstersFromLevelId                                     @10004
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DRLG_GetLevel                                                       @10005

    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DRLG_InitLevel                                                      @10006

    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetRoomExFromRoom                                           @10007
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DRLGPRESET_GetLevelPrestIdFromRoomEx                                @10008
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DRLGPRESET_GetPickedLevelPrestFilePathFromRoomEx                    @10009
    // Don't patch, datatable
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetLevelDefRecord                                          @10010
    // Don't patch, datatable
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetAutomapCellId                                           @10011

    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DRLG_FreeDrlg                                                       @10012
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DRLG_AllocLevel                                                     @10013
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DRLG_AllocDrlg                                                      @10014

    // Uses globals
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DRLGACTIVATE_GetRoomsAllocationStats                                @10015

    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10016_Return0                                              @10016
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DRLGROOMTILE_GetNumberOfShadowsFromRoom                             @10017
    PatchAction::Ignore                        , /*C*/ //   D2COMMON_10018_Return0                                              @10018
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10019_Return                                               @10019
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DRLGPRESET_CreateCopyOfMapAI                                        @10020
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DRLGPRESET_ChangeMapAI                                              @10021
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DRLGPRESET_FreeMapAI                                                @10022

    // Don't patch, datatable
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetLevelTypesTxtRecord                                     @10023
    // Don't patch, datatable
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetLvlPrestTxtRecord                                       @10024

    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetWarpCoordinatesFromRoom                                  @10025
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_ToggleRoomTilesEnableFlag                                   @10026
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetWarpTileFromRoomAndSourceLevelId                         @10027
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetLvlWarpTxtRecordFromRoomAndUnit                          @10028
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetFirstUnitInRoom                                          @10029
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetFloorTilesFromRoom                                       @10030
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetWallTilesFromRoom                                        @10031
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetRoofTilesFromRoom                                        @10032
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetTileDataFromAct                                          @10033
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetRoomCoordinates                                          @10034
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetAdjacentRoomsListFromRoom                                @10035
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetAlliedCountFromRoom                                      @10036
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetTownLevelIdFromAct                                       @10037

    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_AllocAct                                                    @10038
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_FreeAct                                                     @10039

    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_DoRoomsTouchOrOverlap                                       @10040
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_FindRoomBySubtileCoordinates                                @10041
    PatchAction::PointerReplacePatchByOriginal,  /*C*/ //   sgptDataTables                                                      @10042
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_AreTileCoordinatesInsideRoom                                @10043
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_CheckLOSDraw                                                @10044
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_FindActSpawnLocationEx                                      @10045
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_FindActSpawnLocation                                        @10046

    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetHoradricStaffTombLevelId                                 @10047
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_CheckRoomsOverlapping_BROKEN                                                      @10048
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_CallRoomCallback                                            @10049
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetAdjacentRoomByTileCoordinates                                @10050
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_FindRoomByTileCoordinates                                   @10051
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10052                                                      @10052
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetSubtileRect                                              @10053
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetRGB_IntensityFromRoom                                    @10054
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetPresetUnitsFromRoom                                      @10055
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetRoomFromAct                                              @10056
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetLevelIdFromRoom                                          @10057
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetWarpDestinationLevel                                     @10058
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetLevelIdFromPopulatedRoom                                 @10059
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_HasWaypoint                                                 @10060
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetPickedLevelPrestFilePathFromRoom                         @10061
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_ChangeClientRoom                                            @10062
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_SetClientIsInSight                                          @10063
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_UnsetClientIsInSight                                        @10064
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_StreamRoomAtCoords                                          @10065
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_AllocDrlgDelete                                             @10066
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_FreeDrlgDelete                                              @10067
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetDrlgDeleteFromRoom                                       @10068
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetARoomInClientSight                                       @10069
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetARoomInSightButWithoutClient                             @10070
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_TestRoomCanUnTile                                           @10071
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetRoomStatusFlags                                          @10072
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10073                                                      @10073
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10074                                                      @10074
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10075                                                      @10075
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_RemoveRoomFromAct                                           @10076
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10077                                                      @10077
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_UpdateAndFreeInactiveRooms                                                      @10078
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_AddClientToRoom                                             @10079
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_RemoveClientFromRoom                                        @10080
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10081_GetTileCountFromRoom                                 @10081
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_IsRoomInTown                                                @10082
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10083_Return0                                              @10083
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10084                                                      @10084
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetTownLevelIdFromActNo                                     @10085
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_IsTownLevelId                                               @10086
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10087                                                      @10087
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetDrlgFromAct                                              @10088
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetInitSeedFromAct                                          @10089
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetNumberOfPopulatedRoomsInLevel                            @10090
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_UpdateWarpRoomSelect                                        @10091
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_UpdateWarpRoomDeselect                                      @10092
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_UpdatePops                                                  @10093
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetTombStoneTileCoords                                      @10094
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10095                                                      @10095
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10096                                                      @10096
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetRoomCoordList                                            @10097
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetPortalLevelArrayFromPortalFlags                          @10098
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GetPortalFlagFromLevelId                                    @10099
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_IncreaseAlliedCountOfRoom                                   @10100
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_DecreaseAlliedCountOfRoom                                   @10101
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_ToggleHasPortalFlag                                         @10102
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_SetActCallbackFunc                                          @10103
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_AnimateTiles                                                @10104
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_InitRoomTileAnimation                                       @10105
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_SaveKilledUnitGUID                                          @10106
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_ClientToGameTileCoords                                      @10107
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_ClientToGameSubtileCoords                                   @10108
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_ClientToGameCoords                                          @10109
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GameTileToClientCoords                                      @10110
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GameSubtileToClientCoords                                   @10111
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GameToClientCoords                                          @10112
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GameTileToSubtileCoords                                     @10113
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_ClientTileDrawPositionToGameCoords                          @10114
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GameToClientTileDrawPositionCoords                          @10115
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_ClientSubileDrawPositionToGameCoords                        @10116
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DUNGEON_GameToClientSubtileDrawPositionCoords                       @10117
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   COLLISION_CheckMask                                                 @10118
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   COLLISION_CheckMaskWithSize                                         @10119
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   COLLISION_CheckMaskWithSizeXY                                       @10120
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   COLLISION_CheckMaskWithPattern                                      @10121
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   COLLISION_CheckAnyCollisionWithPattern                              @10122
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   COLLISION_ResetMask                                                 @10123
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   COLLISION_ResetMaskWithSize                                         @10124
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   COLLISION_ResetMaskWithSizeXY                                       @10125
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   COLLISION_ResetMaskWithPattern                                      @10126
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   COLLISION_SetMask                                                   @10127
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   COLLISION_SetMaskWithSize                                           @10128
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   COLLISION_SetMaskWithSizeXY                                         @10129
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   COLLISION_SetMaskWithPattern                                        @10130
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   COLLISION_TryMoveUnitCollisionMask                                  @10131
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   COLLISION_TryTeleportUnitCollisionMask                              @10132
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   COLLISION_SetUnitCollisionMask                                      @10133
    PatchAction::FunctionReplacePatchByOriginal,       //   COLLISION_GetFreeCoordinates                                        @10134
    PatchAction::FunctionReplacePatchByOriginal,       //   COLLISION_GetFreeCoordinatesWithMaxDistance                         @10135
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10136                                                      @10136
    PatchAction::FunctionReplacePatchByOriginal,       //   COLLISION_GetFreeCoordinatesEx                                      @10137
    PatchAction::FunctionReplacePatchByOriginal,       //   COLLISION_GetFreeCoordinatesWithField                               @10138
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10139_Return                                               @10139
    PatchAction::Ignore                        , /*C*/ //   D2COMMON_10140_Return                                               @10140
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetClientCoordsVelocity                                        @10141
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10142                                                      @10142
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_SetUnitDeadCollision                                           @10143
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_SetUnitAliveCollision                                          @10144
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_SetUnusedFlag_0x00004                                          @10145
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_SetVelocity                                                    @10146
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetVelocity                                                    @10147
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_SetMaxVelocity                                                 @10148
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetMaxVelocity                                                 @10149
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_SetAcceleration                                                @10150
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetAcceleration                                                @10151
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_AllocDynamicPath                                               @10152
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10153                                                      @10153
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetNumberOfPathPoints                                          @10154
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10155                                                      @10155
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_FreeDynamicPath                                                @10156
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetPathPoints                                                  @10157
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetDirection                                                   @10158
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetNewDirection                                                @10159
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10160_PathUpdateDirection                                  @10160
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_SetDirection                                                   @10161
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetXPosition                                                   @10162
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetYPosition                                                   @10163
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetClientCoordX                                                @10164
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetClientCoordY                                                @10165
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetRoom                                                        @10166
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_SetRoom                                                        @10167
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetNextRoom                                                    @10168
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_ClearNextRoom                                                  @10169
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10170_PathSetTargetPos                                     @10170
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetTargetTypeAndGUID                                           @10171
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_IsCurrentRoomInvalid                                           @10172
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_SetCurrentRoomInvalid                                          @10173
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetUnusedFlag_0x00004                                          @10174
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10175_PathGetFirstPointX                                   @10175
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10176_PathGetFirstPointY                                   @10176
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10177_PATH_GetLastPointX                                   @10177
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10178_PATH_GetLastPointY                                   @10178
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_SetTargetUnit                                                  @10179
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetTargetUnit                                                  @10180
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetFootprintCollisionMask                                      @10181
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_SetFootprintCollisionMask                                      @10182
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetMoveTestCollisionMask                                       @10183
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_SetMoveTestCollisionMask                                       @10184
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_SetType                                                        @10185
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_ResetToPreviousType                                            @10186
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetType                                                        @10187
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_SetNewDistance                                                 @10188
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetMaxDistance                                                 @10189
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   _10190_PATH_SetDistance                                             @10190
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   _10191_PATH_GetDistance                                             @10191
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_SetIDAStarInitFScore											@10192
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10193_PATH_AdjustDirection                                 @10193
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetPrecisionX                                                  @10194
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetPrecisionY                                                  @10195
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_SetPrecisionX                                                  @10196
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_SetPrecisionY                                                  @10197
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10198_PathGetSaveStep                                      @10198
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10199_PathGetSaveX                                         @10199
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10200_PathGetSaveY                                         @10200
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10201                                                      @10201
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10202                                                      @10202
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10203_PATH_SetRotateFlag                                   @10203
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10204_PATH_ClearPoint2                                     @10204
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_SetStepNum                                                     @10205
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetStepNum                                                     @10206
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10207                                                      @10207
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10208_PathSetPathingFlag                                   @10208
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10209_PathCheckPathingFlag                                 @10209
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetUnitCollisionPattern                                        @10210
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_SetUnitCollisionPattern                                        @10211
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10212_PATH_SetMoveFlags                                    @10212
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10213                                                      @10213
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10214                                                      @10214
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_ComputeDirection                                               @10215
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10216                                                      @10216
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_SetDistance                                                    @10217
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_GetDistance                                                    @10218
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_AddToDistance                                                  @10219
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_ComputeSquaredDistance                                         @10220
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_AddCollisionFootprintForOptionalUnit                           @10221
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_AddCollisionFootprintForUnit                                   @10222
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_RemoveCollisionFootprintForUnit                                @10223
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10224                                                      @10224
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10225                                                      @10225
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10226                                                      @10226
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10227                                                      @10227
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10228                                                      @10228
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10229                                                      @10229
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10230                                                      @10230
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10231                                                      @10231
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10232                                                      @10232
    PatchAction::FunctionReplacePatchByOriginal,       //   PATH_RecacheRoomIfNeeded                                            @10233
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10234                                                      @10234
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10235_PATH_UpdateRiderPath                                 @10235
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10236                                                      @10236
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10237                                                      @10237
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_10238_Return                                               @10238
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10239_Return0                                              @10239
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_AllocInventory                                            @10240
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_FreeInventory                                             @10241
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_PlaceItemInSocket                                         @10242
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_RemoveItemFromInventory                                   @10243
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_CompareWithItemsParentInventory                           @10244
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetFreePosition                                           @10245
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_PlaceItemAtFreePosition                                   @10246
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_CanItemBePlaced                                           @10247
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_CanItemsBeExchanged                                       @10248
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_PlaceItemAtInventoryPage                                  @10249
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_Return                                                    @10250
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_10251_Return                                               @10251
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetItemFromInventoryPage                                  @10252
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_PlaceItemInBodyLoc                                        @10253
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_10254_Return0                                              @10254
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetSecondWieldingWeapon                                   @10255
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_CheckEquipmentForWeaponByClass                            @10256
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetItemFromBodyLoc                                        @10257
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetLeftHandWeapon                                         @10258
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetCompositItem                                           @10259
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetBodyLocFromEquippedItem                                @10260
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_SetCursorItem                                             @10261
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetCursorItem                                             @10262
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_FindBackPackItemForStack                                  @10263
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_FindEquippedItemForStack                                  @10264
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_FindFillableBook                                          @10265
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_PlaceItemInBeltSlot                                       @10266
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10267_Return                                               @10267
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_HasSimilarPotionInBelt                                    @10268
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetFreeBeltSlot                                           @10269
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_PlaceItemInFreeBeltSlot                                   @10270
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetItemFromBeltSlot                                       @10271
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetUseableItemFromBeltSlot                                @10272
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetEquippedShield                                         @10273
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetEquippedWeapon                                         @10274
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_HasBodyArmorEquipped                                      @10275
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_IsItemBodyLocFree                                         @10276
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetFirstItem                                              @10277
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetLastItem                                               @10278
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_RemoveInventoryItems                                      @10279
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetTradeInventory                                         @10280
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_FreeTradeInventory                                        @10281
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_CheckForItemInTradeInventory                              @10282
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_AddItemToTradeInventory                                   @10283
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetItemCount                                              @10284
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetBackPackItemByType                                     @10285
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetEquippedItemByType                                     @10286
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetEquippedItemByCode                                     @10287
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetSetItemEquipCountByFileIndex                           @10288
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_UpdateWeaponGUIDOnInsert                                  @10289
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_UpdateWeaponGUIDOnRemoval                                 @10290
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetWieldType                                              @10291
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_SetOwnerId                                                @10292
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetOwnerId                                                @10293
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_CreateCorpseForPlayer                                     @10294
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_FreeCorpse                                                @10295
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetFirstCorpse                                            @10296
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetCorpseCount                                            @10297
    PatchAction::FunctionReplacePatchByOriginal,       //   INVENTORY_GetItemSaveGfxInfo                                        @10298
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10299                                                      @10299
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_RemoveAllItems                                            @10300
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_10301_Return0                                              @10301
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_CanItemsBeTraded                                          @10302
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_10303_Return                                               @10303
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetNextItem                                               @10304
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_UnitIsItem                                                @10305
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetItemGUID                                               @10306
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetItemNodePage                                           @10307
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_10308_Return                                               @10308
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_10309_Return0                                              @10309
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_IsItemInInventory                                         @10310
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetNextNode                                               @10311
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetItemGUIDFromNode                                       @10312
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetNextCorpse                                             @10313
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetUnitGUIDFromCorpse                                     @10314
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10315                                                      @10315
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_10316                                                      @10316
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_CanSwitchAI                                                   @10317
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_IsInMovingMode                                                @10318
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_IsInMovingModeEx                                              @10319
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetStartSkill                                                 @10320
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetLeftSkill                                                  @10321
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetRightSkill                                                 @10322
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetUsedSkill                                                  @10323
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_SetUsedSkill                                                  @10324
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_SetAnimData                                                   @10325
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10326
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetPrecisionX                                                 @10327
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_SetXForStaticUnit                                             @10328
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10329
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetPrecisionY                                                 @10330
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_SetYForStaticUnit                                             @10331
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetCoords                                                     @10332
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetClientCoordX                                               @10333
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetClientCoordY                                               @10334
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetClientCoords                                               @10335
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetUnitSizeX                                                  @10336
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetUnitSizeY                                                  @10337
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetCollisionMask                                              @10338
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetStashGoldLimit                                             @10339
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_SetTargetX                                                    @10340
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_SetTargetY                                                    @10341
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetRoom                                                       @10342
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_ResetRoom                                                     @10343
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_SetTargetUnitForDynamicUnit                                   @10344
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetTargetTypeFromDynamicUnit                                  @10345
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetTargetGUIDFromDynamicUnit                                  @10346
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_SetTargetUnitForPlayerOrMonster                               @10347
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_ChangeAnimMode                                                @10348
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_SetAnimStartFrame                                             @10349
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_InitializeStaticPath                                          @10350
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_BlockCollisionPath                                            @10351
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_FreeCollisionPath                                             @10352
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetNameOffsetFromObject                                       @10353
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetRunAndWalkSpeedForPlayer                                   @10354
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_IsCurrentRoomInvalid                                          @10355
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_SetCurrentRoomInvalid                                         @10356
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_RefreshInventory                                              @10357
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetSkillFromSkillId                                           @10358
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_IsSoftMonster                                                 @10359
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetMeleeRange                                                 @10360
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_IsInMeleeRange                                                @10361
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_TestCollisionWithUnit                                         @10362
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_TestCollisionBetweenInteractingUnits                          @10363
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_TestCollisionByCoordinates                                    @10364
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetHitClass                                                   @10365
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetWeaponClass                                                @10366
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetBeltType                                                   @10367
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetCurrentLifePercentage                                      @10368
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetAnimOrSeqMode                                              @10369
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_SetAnimOrSeqMode                                              @10370
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_InitializeSequence                                            @10371
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_SetAnimationFrame                                             @10372
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_StopSequence                                                  @10373
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_UpdateFrame                                                   @10374
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10375_UNITS_SetFrameNonRate                                @10375
    PatchAction::FunctionReplaceOriginalByPatch,       //   UNITS_UpdateAnimRateAndVelocity                                     @10376
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_SetAnimationSpeed                                             @10377
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_IsAtEndOfFrameCycle                                           @10378
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetShiftedFrameMetrics                                        @10379
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetFrameMetrics                                               @10380
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_SetAnimActionFrame                                            @10381
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetEventFrameInfo                                             @10382
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_ResetLightMap                                                 @10383
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITROOM_AddUnitToRoom                                              @10384
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITROOM_RefreshUnit                                                @10385
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITROOM_RemoveUnitFromRoom                                         @10386
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITROOM_RemoveUnitFromUpdateQueue                                  @10387
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITROOM_SortUnitListByTargetY                                      @10388
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITROOM_IsUnitInRoom                                               @10389
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITROOM_UpdatePath                                                 @10390
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITROOM_ClearUpdateQueue                                           @10391
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_IsDoor                                                        @10392
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_IsShrine                                                      @10393
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetObjectTxtRecordFromObject                                  @10394
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetShrineTxtRecordFromObject                                  @10395
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_SetShrineTxtRecordInObjectData                                @10396
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetDistanceToOtherUnit                                        @10397
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetDistanceToCoordinates                                      @10398
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10399                                                      @10399
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_IsInRange                                                     @10400
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITFINDS_InitializeUnitFindData                                    @10401
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITFINDS_FreeUnitFindData                                          @10402
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITFINDS_FindAllMatchingUnitsInNeighboredRooms                     @10403
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITFINDS_TestUnit                                                  @10404
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITFINDS_GetTestedUnitsFromRoom                                    @10405
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10406                                                      @10406
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10407                                                      @10407
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITFINDS_AreUnitsInNeighboredRooms                                 @10408
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetInventoryRecordId                                          @10409
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_HasCollision                                                  @10410
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetAbsoluteXDistance                                          @10411
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetAbsoluteYDistance                                          @10412
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_UpdateDirectionAndSpeed                                       @10413
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetNewDirection                                               @10414
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_StoreOwner                                                    @10415
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_StoreOwnerTypeAndGUID                                         @10416
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_StoreLastAttacker                                             @10417
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetDirectionToCoords                                          @10418
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_SetInteractData                                               @10419
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_AllocPlayerData                                               @10420
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_FreePlayerData                                                @10421
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_SetNameInPlayerData                                           @10422
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetPlayerName                                                 @10423
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetPlayerData                                                 @10424
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_SetPlayerPortalFlags                                          @10425
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetPlayerPortalFlags                                          @10426
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetObjectPortalFlags                                          @10427
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_SetObjectPortalFlags                                          @10428
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_CheckObjectPortalFlag                                         @10429
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetOverlayHeight                                              @10430
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetDefense                                                    @10431
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetAttackRate                                                 @10432
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetBlockRate                                                  @10433
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10434                                                      @10434
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetEquippedWeaponFromMonster                                  @10435
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetFrameBonus                                                 @10436
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_SetOverlay                                                    @10437
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetHealingCost                                                @10438
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_GetInventoryGoldLimit                                         @10439
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_MergeDualWieldWeaponStatLists                                 @10440
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_10441_Return                                               @10441
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetItemComponentId                                            @10442
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_InitRightSkill                                                @10443
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_InitLeftSkill                                                 @10444
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_InitSwitchRightSkill                                          @10445
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_InitSwitchLeftSkill                                           @10446
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetRightSkillData                                             @10447
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetLeftSkillData                                              @10448
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetSwitchRightSkillDataResetRightSkill                        @10449
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetSwitchLeftSkillDataResetLeftSkill                          @10450
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetSwitchLeftSkillData                                        @10451
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetSwitchRightSkillData                                       @10452
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_SetSwitchLeftSkill                                            @10453
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_SetSwitchRightSkill                                           @10454
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_SetWeaponGUID                                                 @10455
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetWeaponGUID                                                 @10456
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetDirection                                                  @10457
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_SetTimerArg                                                   @10458
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_GetTimerArg                                                   @10459
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_AllocStaticPath                                               @10460
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   UNITS_FreeStaticPath                                                @10461
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_CanDualWield                                                  @10462
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_SetStat                                                    @10463
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_AddStat                                                    @10464
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_SetStatIfListIsValid                                       @10465
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetStatValue                                               @10466
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10467                                                      @10467
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_RemoveAllStats                                             @10468
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10469                                                      @10469
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_AllocStatList                                              @10470
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetOwnerType                                               @10471
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetOwnerGUID                                               @10472
    PatchAction::FunctionReplacePatchByOriginal,       //   D2COMMON_10473                                                      @10473
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10474                                                      @10474
    PatchAction::FunctionReplacePatchByOriginal,       //   D2COMMON_10475_PostStatToStatList                                   @10475
    PatchAction::FunctionReplacePatchByOriginal,       //   D2COMMON_10476                                                      @10476
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_SetStatRemoveCallback                                      @10477
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_SetState                                                   @10478
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetState                                                   @10479
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetStatListFromUnitAndState                                @10480
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetStatListFromUnitAndFlag                                 @10481
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetStatListFromFlag                                        @10482
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetStatListFromUnitStateOrFlag                             @10483
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetStatListFromUnitStateAndFlag                            @10484
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_FreeStatList                                               @10485
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_ToggleState                                                  @10486
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckState                                                   @10487
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_ToggleGfxStateFlag                                           @10488
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckGfxStateFlag                                            @10489
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_IsAnyGfxStateFlagSet                                         @10490
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_ClearGfxStateFlags                                           @10491
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_GetGfxStateFlags                                             @10492
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_UpdateStayDeathFlags                                         @10493
    PatchAction::FunctionReplacePatchByOriginal,       //   D2COMMON_10494_STATES_GetStatFlags                                  @10494
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_GetNumberOfStateFlags                                        @10495
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskAuraByStateId                                  @10496
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskCurseByStateId                                 @10497
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskCurableByStateId                               @10498
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10499
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskPgsvByStateId                                  @10500
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_UpdatePgsvFlags                                              @10501
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskActiveOnUnit                                   @10502
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskAuraOnUnit                                     @10503
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10504
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10505
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10506
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10507
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10508
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10509
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10510
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_FreeModStats                                               @10511
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10512                                                      @10512
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10513                                                      @10513
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_ClampStaminaManaHP                                         @10514
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10515                                                      @10515
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_UpdateStatListsExpiration                                  @10516
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_SetUnitStat                                                @10517
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_AddUnitStat                                                @10518
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_UnitGetStatValue                                           @10519
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_UnitGetItemStatOrSkillStatValue                            @10520
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetUnitBaseStat                                            @10521
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetUnitStatBonus                                           @10522
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_MergeStatLists                                             @10523
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_ExpireUnitStatlist                                         @10524
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10525                                                      @10525
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_AllocStatListEx                                            @10526
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_FreeStatListEx                                             @10527
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_SetExpireFrame                                             @10528
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetExpireFrame                                             @10529
    PatchAction::FunctionReplacePatchByOriginal,       //   D2COMMON_10530_D2CheckStatlistFlagDMGRed                            @10530
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10531_SetStatInStatListLayer0                              @10531
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetTotalStatValue_Layer0                                   @10532
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_RemoveAllStatsFromOverlay                                  @10533
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10534                                                      @10534
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetOwner                                                   @10535
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskBlueOnUnit                                     @10536
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskAttBlueOnUnit                                  @10537
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskDamBlueOnUnit                                  @10538
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskArmBlueOnUnit                                  @10539
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskRFBlueOnUnit                                   @10540
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskRCBlueOnUnit                                   @10541
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskRLBlueOnUnit                                   @10542
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskRPBlueOnUnit                                   @10543
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskAttRedOnUnit                                   @10544
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskDamRedOnUnit                                   @10545
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskArmRedOnUnit                                   @10546
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskRFRedOnUnit                                    @10547
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskRCRedOnUnit                                    @10548
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskRLRedOnUnit                                    @10549
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskRPRedOnUnit                                    @10550
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_IsUnitShapeShifted                                           @10551
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskExpOnUnit                                      @10552
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskExpByStateId                                   @10553
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskStayDeathOnUnitByStateId                       @10554
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskShatterOnUnit                                  @10555
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskShatterByStateId                               @10556
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskHideOnUnit                                     @10557
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskHideByStateId                                  @10558
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskLifeOnUnit                                     @10559
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskUdeadOnUnit                                    @10560
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskGreenOnUnit                                    @10561
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetUnitAlignment                                           @10562
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_AreUnitsAligned                                            @10563
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetMaxLifeFromUnit                                         @10564
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetMaxManaFromUnit                                         @10565
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetMaxStaminaFromUnit                                      @10566
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetMaxDurabilityFromUnit                                   @10567
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetMaxDamageFromUnit                                       @10568
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetMinDamageFromUnit                                       @10569
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetMaxThrowDamageFromUnit                                  @10570
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetMinThrowDamageFromUnit                                  @10571
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetDefenseFromUnit                                         @10572
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_MergeBaseStats                                             @10573
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10574                                                      @10574
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_UnloadAllBins                                              @10575
    PatchAction::FunctionReplaceOriginalByPatch,       //   DATATBLS_LoadAllTxts                                                @10576
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10577
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_CompileTxt                                                 @10578
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_UnloadBin                                                  @10579
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_WriteBinFile                                               @10580
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10581
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10582
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetHirelingTxtRecordFromIdAndLevel                         @10583
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetNextHirelingTxtRecordFromActAndDifficulty               @10584
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetNextHirelingTxtRecordFromNameId                         @10585
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetNextHirelingTxtRecordFromClassId                        @10586
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetNextHirelingTxtRecordFromVendorIdAndDifficulty          @10587
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DATATBLS_GetNpcTxtRecord                                            @10588
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10589
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetMissileVelocityFromMissilesTxt                          @10590
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10591
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetMonsterChainInfo                                        @10592
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetCharstatsTxtTable                                       @10593
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10594
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10595
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DATATBLS_GetArenaTxtRecord                                          @10596
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_MapOldItemIndexToCurrent                                   @10597
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10598
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetItemDataTables                                          @10599
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetItemsTxtRecord                                          @10600
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetItemRecordFromItemCode                                  @10601
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetItemIdFromItemCode                                      @10602
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetMagicAffixDataTables                                    @10603
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetMagicAffixTxtRecord                                     @10604
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetRareAffixDataTables                                     @10605
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetRareAffixTxtRecord                                      @10606
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10607
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10608
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10609
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10610
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetQualityItemDataTables                                   @10611
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetQualityItemsTxtRecord                                   @10612
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetLowQualityItemDataTables                                @10613
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetLowQualityItemsTxtRecord                                @10614
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetGemDataTables                                           @10615
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetGemsTxtRecord                                           @10616
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetBookDataTables                                          @10617
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetBooksTxtRecord                                          @10618
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetRuneDataTables                                          @10619
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetRunesTxtRecord                                          @10620
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_AddOrChangeRunesTxtRecord                                  @10621
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetItemRatioDataTables                                     @10622
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetItemRatioTxtRecord                                      @10623
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetShrinesTxtRecord                                        @10624
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetShrinesTxtRecordCount                                   @10625
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetObjectsTxtRecord                                        @10626
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetObjGroupTxtRecord                                       @10627
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetLevelThreshold                                          @10628
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetMaxLevel                                                @10629
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetCurrentLevelFromExp                                     @10630
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetLevelsTxtRecord                                         @10631
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetRainFromLevelsTxt                                       @10632
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetMudFromLevelsTxt                                        @10633
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetNoPerFromLevelsTxt                                      @10634
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetInventoryRect                                           @10635
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetInventoryGridInfo                                       @10636
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetInventoryComponentGrid                                  @10637
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DATATBLS_GetBeltsTxtRecord                                          @10638
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DATATBLS_GetInvRectFromBeltsTxt                                     @10639
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DATATBLS_UnitAnimInfoDebugSet                                       @10640
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DATATBLS_GetAnimDataInfo                                            @10641
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_SetVelocityInMonStatsTxtRecord                             @10642
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetPlrMode_TypeDataTables                                  @10643
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetMonModeDataTables                                       @10644
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetObjMode_TypeDataTables                                  @10645
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetPlrModeTypeTxtRecord                                    @10646
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetMonModeTxtRecord                                        @10647
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetObjModeTypeTxtRecord                                    @10648
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetCompositTxtRecord                                       @10649
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetArmTypeTxtRecord                                        @10650
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_RollRandomUniqueTitleString                                @10651
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_RollRandomUniquePrefixString                               @10652
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_RollRandomUniqueSuffixString                               @10653
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_RollRandomUniqueAppellationString                          @10654
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetDifficultyLevelsTxtRecord                               @10655
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_ShouldNeverCallInExpansion                                 @10656
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10657
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetTreasureClassExRecordFromName                           @10658
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetTreasureClassExRecordFromIdAndLevel                     @10659
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetTreasureClassExRecordFromActAndDifficulty               @10660
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_10661_Return0                                              @10661
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetMonItemPercentDataTables                                @10662
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetMonItemPercentTxtRecord                                 @10663
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DATATBLS_GetCharTemplateTxtRecordCount                              @10664
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DATATBLS_GetCharTemplateTxtRecord                                   @10665
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DATATBLS_GetClassFromCharTemplateTxtRecord                          @10666
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DATATBLS_UNUSED_Return0                                             @10667
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetSuperUniquesTxtRecord                                   @10668
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10669
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10670
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetGambleDataTables                                        @10671
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10672
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10673
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetFramesFromOverlayTxt                                    @10674
    PatchAction::FunctionReplacePatchByOriginal,       //   DATABLS_GetPreDrawFromOverlayTxt                                    @10675
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetOffsetXFromOverlayTxt                                   @10676
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetOffsetYFromOverlayTxt                                   @10677
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetTransFromOverlayTxt                                     @10678
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetRadiusAndColorFromOverlayTxt                            @10679
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_Get1OfNFromOverlayTxt                                      @10680
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetDirFromOverlayTxt                                       @10681
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DATATBLS_GetMonSeqTxtRecordFromUnit                                 @10682
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DATATBLS_GetSeqFramePointsCount                                     @10683
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DATATBLS_GetSeqFrameCount                                           @10684
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DATATBLS_ComputeSequenceAnimation                                   @10685
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   DATATBLS_GetSequenceEvent                                           @10686
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_AllocItemData                                                 @10687
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_FreeItemData                                                  @10688
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetBodyLocation                                               @10689
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_SetBodyLocation                                               @10690
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetItemSeed                                                   @10691
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_InitItemSeed                                                  @10692
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetItemStartSeed                                              @10693
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_SetItemStartSeed                                              @10694
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetItemQuality                                                @10695
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_SetItemQuality                                                @10696
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetAutoAffix                                                  @10697
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_SetAutoAffix                                                  @10698
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetPrefixId                                                   @10699
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_AssignPrefix                                                  @10700
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetSuffixId                                                   @10701
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_AssignSuffix                                                  @10702
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetRarePrefixId                                               @10703
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_AssignRarePrefix                                              @10704
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetRareSuffixId                                               @10705
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_AssignRareSuffix                                              @10706
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CheckItemFlag                                                 @10707
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_SetItemFlag                                                   @10708
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetItemFlags                                                  @10709
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CheckItemCMDFlag                                              @10710
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_SetItemCMDFlag                                                @10711
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetItemCMDFlags                                               @10712
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10713
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10714
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10715
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10716
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetItemLevel                                                  @10717
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_SetItemLevel                                                  @10718
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetInvPage                                                    @10719
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_SetInvPage                                                    @10720
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetCellOverlap                                                @10721
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_SetCellOverlap                                                @10722
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetEarName                                                    @10723
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_SetEarName                                                    @10724
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetEarLevel                                                   @10725
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_SetEarLevel                                                   @10726
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetVarGfxIndex                                                @10727
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_SetVarGfxIndex                                                @10728
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   ITEMS_CheckItemTypeIdByItemId                                       @10729
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   ITEMS_CheckType                                                     @10730
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   ITEMS_CheckItemTypeId                                               @10731
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetFileIndex                                                  @10732
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_SetFileIndex                                                  @10733
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_SetOwnerId                                                    @10734
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetOwnerId                                                    @10735
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_IsBodyItem                                                    @10736
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_IsClassValid                                                  @10737
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_IsClassValidByItemId                                          @10738
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetClassOfClassSpecificItem                                   @10739
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_IsNotQuestItem                                                @10740
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetQuestFromItemId                                            @10741
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetQuest                                                      @10742
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetNormalCode                                                 @10743
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetWeaponClassCode                                            @10744
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_Get2HandWeaponClassCode                                       @10745
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetBaseCode                                                   @10746
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetAltGfx                                                     @10747
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetComponent                                                  @10748
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetDimensions                                                 @10749
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetAllowedBodyLocations                                       @10750
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetItemType                                                   @10751
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetItemTypeFromItemId                                         @10752
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetItemQlvl                                                   @10753
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CheckIfFlagIsSet                                              @10754
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_SetOrRemoveFlag                                               @10755
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   ITEMS_CheckRequirements                                             @10756
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetLevelRequirement                                           @10757
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CheckBodyLocation                                             @10758
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CheckIfThrowable                                              @10759
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetMissileType                                                @10760
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetMeleeRange                                                 @10761
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CheckItemTypeIfThrowable                                      @10762
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CheckWeaponClassByItemId                                      @10763
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CheckWeaponClass                                              @10764
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CheckWeaponIfTwoHanded                                        @10765
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CheckWeaponIfTwoHandedByItemId                                @10766
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CheckIfStackable                                              @10767
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CheckIfBeltable                                               @10768
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_ComparePotionTypes                                            @10769
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CheckIfAutoBeltable                                           @10770
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CheckIfUseable                                                @10771
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetUniqueColumnFromItemsTxt                                   @10772
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_IsQuestItem                                                   @10773
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_IsNotQuestItemByItemId                                        @10774
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetTransactionCost                                            @10775
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetNoOfUnidItems                                              @10776
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_IsRepairable                                                  @10777
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10778
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10779
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetAmmoTypeFromItemType                                       @10780
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetAmmoType                                                   @10781
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetQuiverTypeFromItemType                                     @10782
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetQuiverType                                                 @10783
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetAutoStackFromItemType                                      @10784
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetAutoStack                                                  @10785
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetReload                                                     @10786
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetReEquip                                                    @10787
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetStorePage                                                  @10788
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetVarInvGfxCount                                             @10789
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetVarInvGfxString                                            @10790
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CanBeMagic                                                    @10791
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CanBeRare                                                     @10792
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CanBeNormal                                                   @10793
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetMaxStack                                                   @10794
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetTotalMaxStack                                              @10795
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetMinStackFromItemId                                         @10796
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetMinStack                                                   @10797
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetSpawnStackFromItemId                                       @10798
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetSpawnStack                                                 @10799
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CheckBitField1Flag8                                           @10800
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CheckBitField1Flag4                                           @10801
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_IsMetalItem                                                   @10802
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CheckBitField1Flag1                                           @10803
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetSpellIcon                                                  @10804
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetDurWarnCount                                               @10805
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetQtyWarnCount                                               @10806
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetStrengthBonus                                              @10807
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetDexBonus                                                   @10808
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CheckIfSocketableByItemId                                     @10809
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CheckIfSocketable                                             @10810
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_HasDurability                                                 @10811
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10812
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetStaffMods                                                  @10813
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetAllowedGemSocketsFromItemId                                @10814
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetMaxSockets                                                 @10815
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetSockets                                                    @10816
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_AddSockets                                                    @10817
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_SetSockets                                                    @10818
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetGemApplyTypeFromItemId                                     @10819
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetGemApplyType                                               @10820
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_IsSocketFiller                                                @10821
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   ITEMS_GetRunesTxtRecordFromItem                                     @10822
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetWeaponClassId                                              @10823
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetTransmogrifyFromItemId                                     @10824
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetTransmogrify                                               @10825
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_IsMagSetRarUniCrfOrTmp                                        @10826
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetHitClassFromItem                                           @10827
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_Is1Or2Handed                                                  @10828
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetColor                                                      @10829
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_IsImbueable                                                   @10830
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_IsSocketable                                                  @10831
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_IsPersonalizable                                              @10832
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_AreStackablesEqual                                            @10833
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CanItemBeUsedForThrowSkill                                    @10834
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_10835_Return0                                              @10835
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetSetItemsMask                                               @10836
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetNoOfSetItemsFromItem                                       @10837
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetSetItemsTxtRecordFromItem                                  @10838
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_CanBeEquipped                                                 @10839
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   ITEMS_IsCharmUsable                                                 @10840
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetBonusLifeBasedOnClass                                      @10841
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetBonusManaBasedOnClass                                      @10842
    PatchAction::FunctionReplacePatchByOriginal,       //   D2COMMON_10843_GetByTimeAdjustment                                  @10843
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10844_ITEMMODS_First                                       @10844
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10845                                                      @10845
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10846                                                      @10846
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMMODS_UpdateItemWithSkillCharges                                 @10847
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10848                                                      @10848
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10849                                                      @10849
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10850                                                      @10850
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10851                                                      @10851
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10852
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetItemCell                                                   @10853
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_SetItemCell                                                   @10854
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMMODS_AssignProperty                                             @10855
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10856_Return                                               @10856
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10857
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10858
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMMODS_CanItemHaveMagicAffix                                      @10859
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMMODS_CanItemHaveRareAffix                                       @10860
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMMODS_CanItemBeHighQuality                                       @10861
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMMODS_SetRandomElixirFileIndex                                   @10862
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10863
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10864
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMMODS_ApplyEthereality                                           @10865
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_UpdateSets                                                    @10866
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMMODS_UpdateRuneword                                             @10867
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMMODS_AddCraftPropertyList                                       @10868
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10870
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10871
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10872
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10873
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10869
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10874
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetItemFormat                                                 @10875
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_SetItemFormat                                                 @10876
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetAllRepairCosts                                             @10877
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetWeaponAttackSpeed                                          @10878
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_HasUsedCharges                                                @10879
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_IsEthereal                                                    @10880
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_SerializeItemToBitstream                                      @10881
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_DecodeItemFromBitstream                                       @10882
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetCompactItemDataFromBitstream                               @10883
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10884_COMPOSIT_unk                                         @10884
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10885_COMPOSIT_unk                                         @10885
    PatchAction::FunctionReplacePatchByOriginal,       //   COMPOSIT_GetWeaponClassIdFromCode                                   @10886
    PatchAction::FunctionReplacePatchByOriginal,       //   COMPOSIT_GetWeaponClassCode                                         @10887
    PatchAction::FunctionReplacePatchByOriginal,       //   COMPOSIT_GetWeaponClassId                                           @10888
    PatchAction::FunctionReplacePatchByOriginal,       //   COMPOSIT_IsArmorComponent                                           @10889
    PatchAction::FunctionReplacePatchByOriginal,       //   COMPOSIT_IsWeaponBowOrXBow                                          @10890
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   COMPOSIT_GetArmorTypeFromComponent                                  @10891
    PatchAction::FunctionReplacePatchByOriginal,       //   CHAT_AllocHoverMsg                                                  @10892
    PatchAction::FunctionReplacePatchByOriginal,       //   CHAT_FreeHoverMsg                                                   @10893
    PatchAction::FunctionReplacePatchByOriginal,       //   CHAT_GetDisplayTimeFromHoverMsg                                     @10894
    PatchAction::FunctionReplacePatchByOriginal,       //   CHAT_GetTimeoutFromHoverMsg                                         @10895
    PatchAction::FunctionReplacePatchByOriginal,       //   CHAT_CopyHoverMsgToBuffer                                           @10896
    PatchAction::FunctionReplacePatchByOriginal,       //   CHAT_GetUsedFromHoverMsg                                            @10897
    PatchAction::FunctionReplacePatchByOriginal,       //   CHAT_SetUsedInHoverMsg                                              @10898
    PatchAction::FunctionReplacePatchByOriginal,       //   CHAT_GetLangIdFromHoverMsg                                          @10899
    PatchAction::FunctionReplacePatchByOriginal,       //   CHAT_SetLangIdInHoverMsg                                            @10900
    PatchAction::FunctionReplacePatchByOriginal,       //   TEXT_AllocTextHeader                                                @10901
    PatchAction::FunctionReplacePatchByOriginal,       //   TEXT_FreeTextHeader                                                 @10902
    PatchAction::FunctionReplacePatchByOriginal,       //   TEXT_AddNodeToTextList                                              @10903
    PatchAction::FunctionReplacePatchByOriginal,       //   TEXT_RemoveNodeFromTextList                                         @10904
    PatchAction::FunctionReplacePatchByOriginal,       //   TEXT_GetMenuType2NodeCount                                          @10905
    PatchAction::FunctionReplacePatchByOriginal,       //   TEXT_GetStringIdOfMenuType2Node                                     @10906
    PatchAction::FunctionReplacePatchByOriginal,       //   TEXT_GetStringIdOfMenuType1Or2Node                                  @10907
    PatchAction::FunctionReplacePatchByOriginal,       //   TEXT_GetStringIdOfMenuType0Or2Node                                  @10908
    PatchAction::FunctionReplacePatchByOriginal,       //   TEXT_CreateMessageListFromTextHeader                                @10909
    PatchAction::FunctionReplacePatchByOriginal,       //   TEXT_CreateTextHeaderFromMessageList                                @10910
    PatchAction::FunctionReplacePatchByOriginal,       //   TEXT_SortTextNodeListByStringId                                     @10911
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   SEED_InitSeed                                                       @10912
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   SEED_InitLowSeed                                                    @10913
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   SEED_GetLowSeed                                                     @10914
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   SEED_GetHighSeed                                                    @10915
    PatchAction::Ignore,                         /*C*/ //   SEED_Return                                                         @10916
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10917
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10918
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10919
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   SEED_GetRandomValue                                                 @10920
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   SEED_SetSeeds                                                       @10921
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   SEED_GetSeeds                                                       @10922
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   ENVIRONMENT_UpdatePeriodOfDay                                       @10923
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   ENVIRONMENT_UpdateCycleIndex                                        @10924
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   ENVIRONMENT_GetStatistics                                           @10925
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   ENVIRONMENT_GetIntensityFromAct                                     @10926
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   ENVIRONMENT_GetLightColorFromAct                                    @10927
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   ENVIRONMENT_GetUnusedMember                                         @10928
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   ENVIRONMENT_NextEnvCycle                                            @10929
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   ENVIRONMENT_GetCycleIndexFromAct                                    @10930
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   ENVIRONMENT_GetCycleIndex_Ticks_EclipseFromAct                      @10931
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   ENVIRONMENT_InitializeEnvironment                                   @10932
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   ENVIRONMENT_GetPeriodOfDayFromAct                                   @10933
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   ENVIRONMENT_GetTimeRateFromAct                                      @10934
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   ENVIRONMENT_SetNextTimeRate                                         @10935
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   ENVIRONMENT_TaintedSunBegin                                         @10936
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   ENVIRONMENT_TaintedSunEnd                                           @10937
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetPassiveState                                              @10938
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10939
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_RefreshSkill                                                 @10940
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_RefreshPassiveSkills                                         @10941
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetSkillModeFromUnit                                         @10942
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_10943_Return1                                              @10943
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetRange                                                     @10944
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_AllocSkillList                                               @10945
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_InitSkillList                                                @10946
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetNextSkill                                                 @10947
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetFirstSkillFromSkillList                                   @10948
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetSkillById                                                 @10949
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetHighestLevelSkillFromUnitAndId                            @10950
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_RemoveSkill                                                  @10951
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_AddSkill                                                     @10952
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_AssignSkill                                                  @10953
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10954                                                      @10954
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetSkillInfo                                                 @10955
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_SetCharges                                                   @10956
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetOwnerGUIDFromSkill                                        @10957
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10958                                                      @10958
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10959                                                      @10959
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_FreeSkillList                                                @10960
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_SetLeftActiveSkill                                           @10961
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_SetRightActiveSkill                                          @10962
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetSkillIdFromSkill                                          @10963
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetUseState                                                  @10964
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetSeqNumFromSkill                                           @10965
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetSkillsTxtRecordFromSkill                                  @10966
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetShrineSkillLevelBonus                                     @10967
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetSkillLevel                                                @10968
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10969                                                      @10969
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10970                                                      @10970
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10971                                                      @10971
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10972                                                      @10972
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_10973                                                      @10973
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_SetSkillMode                                                 @10974
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetSkillMode                                                 @10975
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetParam1                                                    @10976
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetParam2                                                    @10977
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetParam3                                                    @10978
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetParam4                                                    @10979
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_SetParam1                                                    @10980
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_SetParam2                                                    @10981
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_SetParam3                                                    @10982
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_SetParam4                                                    @10983
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_SetFlags                                                     @10984
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetFlags                                                     @10985
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   SKILLS_GetRequiredLevel                                             @10986
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   SKILLS_GetRequiredLevelBasedOnCurrent                               @10987
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   SKILLS_CheckRequiredSkills                                          @10988
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   SKILLS_CheckRequiredAttributes                                      @10989
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10990
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10991
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10992
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10993
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10994
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10995
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10996
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10997
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @10998
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetToHitFactor                                               @10999
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetHighestSkillLevelById                                     @11000
    PatchAction::FunctionReplacePatchByOriginal, /*B*/ //   SKILLS_GetManaCosts                                                 @11001
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   SKILLS_GetMinPhysDamage                                             @11002
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   SKILLS_GetMaxPhysDamage                                             @11003
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   SKILLS_GetMinElemDamage                                             @11004
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   SKILLS_GetMaxElemDamage                                             @11005
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   SKILLS_GetElementalLength                                           @11006
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11007
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetClassIdFromSkillId                                        @11008
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_11009_Return0                                              @11009
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_IsPlayerClassSkill                                           @11010
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetQuantity                                                  @11011
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_SetQuantity                                                  @11012
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2COMMON_11013_ConvertMode                                          @11013
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   D2Common_11014_ConvertShapeShiftedMode                              @11014
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11015                                                      @11015
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11016                                                      @11016
    PatchAction::FunctionReplacePatchByOriginal,       //   D2COMMON_11017_CheckUnitIfConsumeable                               @11017
    PatchAction::FunctionReplacePatchByOriginal,       //   D2COMMON_11018                                                      @11018
    PatchAction::FunctionReplacePatchByOriginal,       //   D2COMMON_11019                                                      @11019
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11020                                                      @11020
    PatchAction::FunctionReplacePatchByOriginal,       //   D2COMMON_11021                                                      @11021
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_CanUnitCorpseBeSelected                                      @11022
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetWeaponMasteryBonus                                        @11023
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11024                                                      @11024
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11025                                                      @11025
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11026                                                      @11026
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetShiftedManaCosts                                          @11027
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11028                                                      @11028
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetBonusSkillLevelFromSkillId                                @11029
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11030                                                      @11030
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11031                                                      @11031
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11032                                                      @11032
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11033                                                      @11033
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11034                                                      @11034
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11035                                                      @11035
    PatchAction::FunctionReplacePatchByOriginal,       //   D2COMMON_11036_GetMonCurseResistanceSubtraction                     @11036
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_CheckIfCanLeapTo                                             @11037
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11038
    PatchAction::FunctionReplacePatchByOriginal,       //   D2COMMON_11039_CheckWeaponIsMissileBased                            @11039
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_IsEnhanceable                                                @11040
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetClassSkillId                                              @11041
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetPlayerSkillCount                                          @11042
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11043                                                      @11043
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11044
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11045
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11046
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetConcentrationDamageBonus                                  @11047
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_11048_Return1                                              @11048
    PatchAction::Ignore,                         /*C*/ //   SKILLS_Return1                                                      @11049
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11050                                                      @11050
    PatchAction::FunctionReplacePatchByOriginal,       //   MONSTERS_GetMaximalLightRadius                                      @11051
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11052                                                      @11052
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11053                                                      @11053
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11054                                                      @11054
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11055                                                      @11055
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11056
    PatchAction::FunctionReplacePatchByOriginal,       //   MONSTERS_IsSandLeaper                                               @11057
    PatchAction::FunctionReplacePatchByOriginal,       //   MONSTERS_IsDemon                                                    @11058
    PatchAction::FunctionReplacePatchByOriginal,       //   MONSTERS_IsUndead                                                   @11059
    PatchAction::FunctionReplacePatchByOriginal,       //   MONSTERS_IsBoss                                                     @11060
    PatchAction::FunctionReplacePatchByOriginal,       //   MONSTERS_GetMinionSpawnInfo                                         @11061
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_11062_Return                                               @11062
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11063                                                      @11063
    PatchAction::FunctionReplacePatchByOriginal,       //   MONSTERS_IsDead                                                     @11064
    PatchAction::FunctionReplacePatchByOriginal,       //   MONSTERS_IsPrimeEvil                                                @11065
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11066                                                      @11066
    PatchAction::FunctionReplacePatchByOriginal,       //   MONSTERS_GetHirelingTypeId                                          @11067
    PatchAction::FunctionReplacePatchByOriginal,       //   D2COMMON_11068_GetCompInfo                                          @11068
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11069                                                      @11069
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11070                                                      @11070
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11071
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11072
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11073
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11074
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11075
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_11076_Return                                               @11076
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_11077_Return                                               @11077
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_11078_Return0                                              @11078
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_11079_Return0                                              @11079
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_11080_Return                                               @11080
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11081                                                      @11081
    PatchAction::FunctionReplacePatchByOriginal,       //   MONSTERS_HirelingInit                                               @11082
    PatchAction::FunctionReplacePatchByOriginal,       //   MONSTERS_GetHirelingResurrectionCost                                @11083
    PatchAction::FunctionReplacePatchByOriginal,       //   MONSTERS_GetHirelingExpForNextLevel                                 @11084
    PatchAction::FunctionReplacePatchByOriginal,       //   MONSTERS_GetHirelingDescString                                      @11085
    PatchAction::FunctionReplacePatchByOriginal,       //   MONSTERS_GetActFromHirelingTxt                                      @11086
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITFINDS_FindUnitInNeighboredRooms                                 @11087
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITFINDS_GetNearestTestedUnit                                      @11088
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_InitializeCollisionFieldTable                              @11089
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_FreeCollisionFieldTable                                    @11090
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetCollisionFieldCenter                                    @11091
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetCollisionFieldWidth                                     @11092
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetCollisionFieldHeight                                    @11093
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_AllocField                                                 @11094
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_FreeField                                                  @11095
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_SetFieldCoordinates                                        @11096
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11097                                                      @11097
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11098                                                      @11098
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11099                                                      @11099
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   LOG_11100                                                           @11100
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   LOG_11101                                                           @11101
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   LOG_11102                                                           @11102
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   LOG_11103                                                           @11103
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   LOG_11104                                                           @11104
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   LOG_11105                                                           @11105
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   LOG_11106                                                           @11106
    PatchAction::FunctionReplacePatchByOriginal,       //   QUESTRECORD_GetQuestState                                           @11107
    PatchAction::FunctionReplacePatchByOriginal,       //   QUESTRECORD_SetQuestState                                           @11108
    PatchAction::FunctionReplacePatchByOriginal,       //   QUESTRECORD_ClearQuestState                                         @11109
    PatchAction::FunctionReplacePatchByOriginal,       //   QUESTRECORD_ResetIntermediateStateFlags                             @11110
    PatchAction::FunctionReplacePatchByOriginal,       //   QUESTRECORD_AllocRecord                                             @11111
    PatchAction::FunctionReplacePatchByOriginal,       //   QUESTRECORD_FreeRecord                                              @11112
    PatchAction::FunctionReplacePatchByOriginal,       //   QUESTRECORD_CopyBufferToRecord                                      @11113
    PatchAction::FunctionReplacePatchByOriginal,       //   QUESTRECORD_CopyRecordToBuffer                                      @11114
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_AllocMissileData                                            @11115
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_FreeMissileData                                             @11116
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetFlags                                                    @11117
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_SetFlags                                                    @11118
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_SetLevel                                                    @11119
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetLevel                                                    @11120
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_SetTotalFrames                                              @11121
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetTotalFrames                                              @11122
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_SetCurrentFrame                                             @11123
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetCurrentFrame                                             @11124
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetRemainingFrames                                          @11125
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_SetSkill                                                    @11126
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetSkill                                                    @11127
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetClassId                                                  @11128
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_SetOwner                                                    @11129
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_CheckUnitIfOwner                                            @11130
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_SetStreamMissile                                            @11131
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetStreamMissile                                            @11132
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_SetStreamRange                                              @11133
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetStreamRange                                              @11134
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetHitClass                                                 @11135
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_SetActivateFrame                                            @11136
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetActivateFrame                                            @11137
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetAnimSpeed                                                @11138
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_SetAnimSpeed                                                @11139
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_SetTargetX                                                  @11140
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetTargetX                                                  @11141
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_SetTargetY                                                  @11142
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetTargetY                                                  @11143
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_SetHomeType                                                 @11144
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetHomeType                                                 @11145
    PatchAction::FunctionReplacePatchByOriginal,       //   WAYPOINTS_IsActivated                                               @11146
    PatchAction::FunctionReplacePatchByOriginal,       //   WAYPOINTS_ActivateWaypoint                                          @11147
    PatchAction::FunctionReplacePatchByOriginal,       //   WAYPOINTS_AllocWaypointData                                         @11148
    PatchAction::FunctionReplacePatchByOriginal,       //   WAYPOINTS_FreeWaypointData                                          @11149
    PatchAction::FunctionReplacePatchByOriginal,       //   WAYPOINTS_CopyAndValidateWaypointData                               @11150
    PatchAction::FunctionReplacePatchByOriginal,       //   WAYPOINTS_ValidateAndCopyWaypointData                               @11151
    PatchAction::FunctionReplacePatchByOriginal,       //   WAYPOINTS_GetWaypointNoFromLevelId                                  @11152
    PatchAction::FunctionReplacePatchByOriginal,       //   WAYPOINTS_GetLevelIdFromWaypointNo                                  @11153
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11155
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11154
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11156
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11157
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11158
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11159
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11160
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11161
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11162
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11163
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11164
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11165
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11166
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11167
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11168
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11169
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11170
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11171
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11172
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11173
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11174
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11175
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11176
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11177
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11178
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11179
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11180
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11181
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11182
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11183
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11184
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11185
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11186
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11187
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11188
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11189
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11190
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11191
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11192
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11193
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11194
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11195
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11196
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11197
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11198
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11199
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11200
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11201
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11202
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11203
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11204
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11205
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11206
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11207
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11208
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11209
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11210
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11211
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11212
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSTREAM_AllocMissileStream                                        @11213
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSTREAM_FreeMissileStream                                         @11214
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSTREAM_ExecuteHit                                                @11215
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSTREAM_Update                                                    @11216
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_CalculateDamageData                                         @11217
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_SetDamageStats                                              @11218
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11219
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11220
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetElementalLength                                          @11221
    PatchAction::FunctionReplacePatchByOriginal,       //   DRLGPRESET_CountPresetObjectsByAct                                  @11222
    PatchAction::FunctionReplacePatchByOriginal,       //   DRLGPRESET_GetObjectIndexFromObjPreset                              @11223
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11224
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11225
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_CheckActInLevelTypesTxt                                    @11226
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetFileNameFromLevelTypeAndFileId                          @11227
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11228
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_11229_Return                                               @11229
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_RemoveTransformStatesFromShapeshiftedUnit                    @11230
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11231
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetCubemainTxtRecord                                       @11232
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetCubemainTxtRecordCount                                  @11233
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskPgsvOnUnit                                     @11234
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_11235_Return0                                              @11235
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_11236_Return                                               @11236
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_11237_Return                                               @11237
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_IsCorpseUseable                                               @11238
    PatchAction::Ignore,                         /*C*/ //   SKILLS_Return0                                                      @11239
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_11240_Return                                               @11240
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_11241_Return                                               @11241
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_ToggleCompileTxtFlag                                       @11242
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetBaseStatsData                                           @11243
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_GetRealmData                                                  @11244
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMS_SetRealmData                                                  @11245
    PatchAction::FunctionReplacePatchByOriginal,       //   MONSTERS_ApplyClassicScaling                                        @11246
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetMonsterLevelInArea                                      @11247
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11248                                                      @11248
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetExpRatio                                                @11249
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetMonSoundsTxtRecordFromMonsterId                         @11250
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11251
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetMonSoundsTxtRecordFromSoundId                           @11252
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11253
    PatchAction::Ignore,                         /*C*/ //; ------------------------UNUSED------------------------               @11254
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetCodeFromCompCodeTxt                                     @11255
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetMonPresetTxtActSection                                  @11256
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetSuperUniquesTxtRecordCount                              @11257
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_MapSuperUniqueId                                           @11258
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_AllocUnit                                                     @11259
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_FreeUnit                                                      @11260
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_CalculateMonsterStatsByLevel                               @11261
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetMonSeqTableRecord                                       @11262
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   COLLISION_RayTrace                                                      @11263
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_SetSkillId                                                 @11264
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetSkillId                                                 @11265
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_SetSkillLevel                                              @11266
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetSkillLevel                                              @11267
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetFullStatsDataFromUnit                                   @11268
    PatchAction::FunctionReplacePatchByOriginal,       //   D2COMMON_11269_CopyStats                                            @11269
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_CopyStats                                                      @11270
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_GetSpecialParamValue                                         @11271
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskRemHitOnUnit                                   @11272
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11273                                                      @11273
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11274                                                      @11274
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11275                                                      @11275
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_EvaluateSkillFormula                                         @11276
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_11277_Return                                               @11277
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetItemsXPosition                                         @11278
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITROOM_AddUnitToRoomEx                                            @11279
    PatchAction::FunctionReplacePatchByOriginal,       //   MONSTERS_GetSpawnMode_XY                                            @11280
    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11281_CollisionPatternFromSize                                               @11281
    PatchAction::FunctionReplacePatchByOriginal,       //   PATH_GetCollisionPatternFromMonStats2Txt                            @11282
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_CalculateKickDamage                                          @11283
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_EvaluateMissileFormula                                      @11284
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetMinDamage                                                @11285
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetMaxDamage                                                @11286
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetMinElemDamage                                            @11287
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetMaxElemDamage                                            @11288
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetElemTypeFromMissileId                                    @11289
    PatchAction::FunctionReplacePatchByOriginal,       //   MISSILE_GetSpecialParamValue                                        @11290
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   PATH_SetNumberOfPathPoints                                                      @11291
    PatchAction::FunctionReplacePatchByOriginal,       //   D2COMMON_11292_ItemAssignProperty                                   @11292
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMMODS_GetItemCharges                                             @11293
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_SetBaseStat                                                @11294
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_SetBaseStat2                                               @11295
    PatchAction::Ignore,                         /*C*/ //   D2COMMON_11296_Return                                               @11296
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   MONSTERS_SetMonsterNameInMonsterData                                @11297
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetUnitNameFromUnit                                        @11298
    PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetUnitNameFromUnitTypeAndClassId                          @11299
    PatchAction::FunctionReplacePatchByOriginal,       //   ITEMMODS_EvaluateItemFormula                                        @11300
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetSecondaryWeapon                                        @11301
    PatchAction::FunctionReplacePatchByOriginal,       //   SKILLS_EvaluateSkillDescFormula                                     @11302
    PatchAction::FunctionReplacePatchByOriginal,       //   STATES_CheckStateMaskOnUnit                                         @11303
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetBaseStatsCount                                          @11304
    PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_GetFullStatsCountFromUnit                                  @11305
    PatchAction::FunctionReplaceOriginalByPatch, /*C*/ //   INVENTORY_GetBackPackItemByCode                                     @11306
    PatchAction::FunctionReplacePatchByOriginal,       //   UNITS_IsObjectInInteractRange                                       @11307

};

extern "C" {

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

static ExtraPatchAction extraPatchActions[] = {
#ifdef D2_VERSION_110F
	// We need to cover all usage of gpLevelFilesList_6FDEA700 and gpLvlSubTypeFilesCriticalSection
    //{ 0x6FDEA700 - D2CommonImageBase, &gpLevelFilesList_6FDEA700, PatchAction::PointerReplaceOriginalByPatch },
    { 0x6FD86050 - D2CommonImageBase, &DRLGPRESET_LoadDrlgFile, PatchAction::FunctionReplaceOriginalByPatch },
    { 0x6FD86190 - D2CommonImageBase, &DRLGPRESET_FreeDrlgFile, PatchAction::FunctionReplaceOriginalByPatch },
    { 0x6FD87F20 - D2CommonImageBase, &DRLGPRESET_FreeDrlgMap, PatchAction::FunctionReplaceOriginalByPatch },
    { 0x6FD62020 - D2CommonImageBase, &DATATBLS_LoadLvlSubTxt, PatchAction::FunctionReplaceOriginalByPatch },
    { 0x6FD62600 - D2CommonImageBase, &DATATBLS_UnloadLvlSubTxt, PatchAction::FunctionReplaceOriginalByPatch },

    // Only ever accessed through DATATBLS_LoadArenaTxt, DATATBLS_GetArenaTxtRecord and DATATBLS_UnloadArenaTxt
    //{ 0x6FDE9600 - D2CommonImageBase, &gpArenaTxtTable, PatchAction::PointerReplaceOriginalByPatch },
    { 0x6FD47840 - D2CommonImageBase, &DATATBLS_LoadArenaTxt, PatchAction::FunctionReplaceOriginalByPatch },
    { 0x6FD479B0 - D2CommonImageBase, &DATATBLS_UnloadArenaTxt, PatchAction::FunctionReplaceOriginalByPatch },
    
    // Only ever accessed through DATATBLS_LoadCharTemplateTxt, DATATBLS_GetCharTemplateTxtRecord, DATATBLS_GetClassFromCharTemplateTxtRecord and DATATBLS_UnloadCharTemplateTxt
    // Also need to have the count matching with DATATBLS_GetCharTemplateTxtRecordCount
    //{ 0x6FDE95F8 - D2CommonImageBase, &gpCharTemplateTxtTable, PatchAction::PointerReplaceOriginalByPatch },
    { 0x6FD479D0 - D2CommonImageBase, &DATATBLS_LoadCharTemplateTxt, PatchAction::FunctionReplaceOriginalByPatch },
    { 0x6FD48770 - D2CommonImageBase, &DATATBLS_UnloadCharTemplateTxt, PatchAction::FunctionReplaceOriginalByPatch },

    // Only ever accessed through DATATBLS_LoadCharTemplateTxt, DATATBLS_GetBeltsTxtRecord, DATATBLS_GetInvRectFromBeltsTxt and DATATBLS_UnloadBeltsTxt
    //{ 0x6FDEA704 - D2CommonImageBase, &sgpBeltsTxtTable, PatchAction::PointerReplaceOriginalByPatch },
    { 0x6FD48880 - D2CommonImageBase, &DATATBLS_LoadCharTemplateTxt, PatchAction::FunctionReplaceOriginalByPatch },
    { 0x6FD493A0 - D2CommonImageBase, &DATATBLS_UnloadBeltsTxt, PatchAction::FunctionReplaceOriginalByPatch },
    
    // Can patch directly since it's pointing to the global static data tables
    { 0x6FDDAF34 - D2CommonImageBase, &gpAutomapSeed, PatchAction::PointerReplacePatchByOriginal},

    // Known broken (or at least unable to function without patching other functions/variables) functions

    // Fixed or working
    //{ 0x6FD83E20 - D2CommonImageBase, &DRLGOUTROOM_InitializeDrlgOutdoorRoom, PatchAction::PointerReplaceOriginalByPatch},
    //{ 0x6FD8ACE0 - D2CommonImageBase, &sub_6FD8ACE0, PatchAction::FunctionReplacePatchByOriginal},
    //{ 0x6FD7EFE0 - D2CommonImageBase, &DRLG_OUTDOORS_GenerateDirtPath, PatchAction::FunctionReplacePatchByOriginal},  
    
    // this is the one that leads to issues
    //{ 0x6FD73450 - D2CommonImageBase, &DRLGACTIVATE_RoomExSetStatus_ClientInSight, PatchAction::FunctionReplacePatchByOriginal},
#endif
    { 0, 0, PatchAction::Ignore}, // Here because we need at least one element in the array
};

constexpr int __cdecl GetExtraPatchActionsCount() { 
#ifdef DISABLE_ALL_PATCHES
    return 0;
#else
    return sizeof(extraPatchActions) / sizeof(ExtraPatchAction); 
#endif
}

ExtraPatchAction* __cdecl GetExtraPatchAction(int index)
{
    return &extraPatchActions[index];
}

__declspec(dllexport)
PatchInformationFunctions __cdecl GetPatchInformationFunctions(const wchar_t* dllName)
{
	D2_MAYBE_UNUSED(dllName);
	return { &GetBaseOrdinal, &GetLastOrdinal, &GetPatchAction, &GetExtraPatchActionsCount, &GetExtraPatchAction };
}

}

#include <type_traits>
static_assert(std::is_same<decltype(GetBaseOrdinal)*, GetIntegerFunctionType>::value, "Ensure calling convention doesn't change");
static_assert(std::is_same<decltype(GetLastOrdinal)*, GetIntegerFunctionType>::value, "Ensure calling convention doesn't change");
static_assert(std::is_same<decltype(GetPatchAction)*, GetPatchActionType>::value, "Ensure calling convention doesn't change");


static_assert(std::is_same<decltype(GetExtraPatchActionsCount)*, GetIntegerFunctionType>::value, "Ensure calling convention doesn't change");
static_assert(std::is_same<decltype(GetExtraPatchAction)*, GetExtraPatchActionType>::value, "Ensure calling convention doesn't change");

static_assert(std::is_same<decltype(GetPatchInformationFunctions)*, GetPatchInformationFunctionsType>::value, "Ensure calling convention doesn't change");
