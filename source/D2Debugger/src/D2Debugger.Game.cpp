#include <imgui.h>
#include <vector>
#include "D2Debugger.h"
#include <Game/Game.h>
#include <D2Lang.h>
#include <D2Unicode.h>
#include <D2Dll.h>
#include <D2DataTbls.h>
#include <D2StatList.h>

#include "IconsFontAwesome6.h"

#include <Windows.h>
extern HMODULE DLLBASE_D2Game;
HMODULE delayedD2GameDllBaseGet()
{
    static HMODULE DLLBASE_D2Game = LoadLibraryA("D2Game.dll");
    return DLLBASE_D2Game;
}

static const int D2GameImageBase = 0x6FC30000;
D2FUNC(D2Game, SpawnSuperUnique_6FC6F690, D2UnitStrc*, __fastcall, (D2GameStrc* pGame, D2RoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nSuperUnique), 0x6FC6F690 - D2GameImageBase);
D2FUNC(D2Game, SpawnMonster_6FC69F10, D2UnitStrc*, __fastcall, (D2GameStrc* pGame, D2RoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nMonsterId, int32_t nAnimMode, int32_t a7, int16_t nFlags), 0x6FC69F10 - D2GameImageBase);


// Using a define so that we break inline
#define AddDebugBreakButton() do{ if (ImGui::Button(ICON_FA_HAMMER)) { __debugbreak(); }; } while(false)

std::vector<char> GetUTF8CharBufferFromStringIndex(uint16_t index)
{
    const Unicode* nameUnicode = (const Unicode*)D2LANG_GetStringFromTblIndex(index);
    std::vector<char> utf8CharBuffer(Unicode::strlen(nameUnicode) * 3, 0);
    Unicode::toUtf(utf8CharBuffer.data(), nameUnicode, utf8CharBuffer.size() - 2 /*See toUtf doc*/);
    return utf8CharBuffer;
}

// Note: Missile and items are inverted on purpose here, this is like this in the game
int GAME_RemapUnitTypeToListIndex(const D2C_UnitTypes nUnitType)
{
    switch (nUnitType)
    {
    case UNIT_MISSILE: return 4;
    case UNIT_ITEM: return 3;
    default: return (int)nUnitType;
    }
}

static const char* gActNames[] = {
    "Act 1",
    "Act 2",
    "Act 3",
    "Act 4",
    "Act 5",
    "Act *", // Used as a sentinel value
};

template<typename ItFunction>
void DebutIterateUnitList(D2GameStrc* pGame, const D2C_UnitTypes nUnitType, ItFunction&& itFunc)
{
    ImGui::PushID(nUnitType);

    ImGui::Text("Filters:");

    static const int dwordInputSize = ImGui::CalcTextSize("-00000000").x;
    static int nClassIdFilter = -1;
    static D2UnitGUID nUnitGUIDFilter = D2UnitInvalidGUID;
    static int nActFilter = D2C_Acts::NUM_ACTS;

    ImGui::PushItemWidth(dwordInputSize);
    ImGui::SameLine(); ImGui::InputInt("ClassId", &nClassIdFilter, 0, 0);
    ImGui::SameLine(); ImGui::InputInt("GUID", (int*)&nUnitGUIDFilter, 0,0);
    ImGui::PopItemWidth();

    static const int ActComboWidth = ImGui::CalcTextSize("Act X").x + /*arrow*/ ImGui::GetFrameHeight() + ImGui::GetStyle().FramePadding.x * 2;
    ImGui::SetNextItemWidth(ActComboWidth);
    ImGui::SameLine(); ImGui::Combo("##Act", (int*)&nActFilter, gActNames, ARRAY_SIZE(gActNames));

    // This is pretty crappy, we are forced to iterate all units to get the total number of units to display
    // This is more or less required as soon as we filter the list anyway...
    static std::vector<D2UnitStrc*> filteredUnits;
    filteredUnits.clear();
    filteredUnits.reserve(1024);

    D2UnitStrc** pUnitList = pGame->pUnitList[GAME_RemapUnitTypeToListIndex(nUnitType)];
    const int32_t nFirstHashIdx = (nUnitGUIDFilter != D2UnitInvalidGUID) ? (nUnitGUIDFilter & 0x7F) : 0;
    const int32_t nLastHashIdx = (nUnitGUIDFilter != D2UnitInvalidGUID) ? (nUnitGUIDFilter & 0x7F) : 0x7F;
    bool stopIteration = false;
    for (int32_t i = nFirstHashIdx; i <= nLastHashIdx && !stopIteration; ++i)
    {
        for (D2UnitStrc* pUnit = pUnitList[i];
            pUnit != nullptr && !stopIteration;
            pUnit = pUnit->pListNext)
        {
            if ((nUnitGUIDFilter != D2UnitInvalidGUID) && (pUnit->dwUnitId != nUnitGUIDFilter))
                continue;
            if ((nClassIdFilter >= 0) && (pUnit->dwClassId != nClassIdFilter))
                continue;
            if ((nActFilter != D2C_Acts::NUM_ACTS) && (pUnit->nAct != nActFilter))
                continue;

            if (nUnitGUIDFilter != D2UnitInvalidGUID)
            {
                // This is the only case where we can early out
                itFunc(pUnit);
                stopIteration = true;
            }
            else
            {
                filteredUnits.push_back(pUnit);
            }
        }
    }

    ImGui::Text("Count: %d", filteredUnits.size());
    if (ImGui::BeginChild("##ScrollingRegion", ImVec2(0, 500), false, ImGuiWindowFlags_AlwaysVerticalScrollbar))
    {
        ImGuiListClipper clipper;
        clipper.Begin(filteredUnits.size());
        while (clipper.Step() && !stopIteration)
        {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd && !stopIteration; i++)
            {
                if (!itFunc(filteredUnits[i]))
                {
                    stopIteration = true;
                }
            }
        }
        clipper.End();

    }
    ImGui::EndChild();
    ImGui::PopID();

}

void D2DebugUnitAnim(D2UnitStrc * pUnit)
{
    if (pUnit->pAnimData)
    {
        ImGui::BulletText("Name           %8s", pUnit->pAnimData->szAnimDataName);
        ImGui::BulletText("Frames         %d", pUnit->pAnimData->dwFrames);
        ImGui::BulletText("Base Speed     %d", pUnit->pAnimData->dwAnimSpeed);
    }
    ImGui::BulletText("Mode           %d", pUnit->dwAnimMode);
    ImGui::BulletText("Speed          %d", pUnit->wAnimSpeed);
    ImGui::BulletText("FrameCount     %d", pUnit->dwFrameCountPrecise);
    ImGui::BulletText("nActionFrame   %d", pUnit->nActionFrame);
    ImGui::BeginDisabled(pUnit->pAnimSeq == nullptr);
    ImGui::BulletText("Sequence Mode  %d", pUnit->dwSeqMode);
    ImGui::BulletText("Sequence Speed %d", pUnit->dwSeqSpeed);
    ImGui::BulletText("Sequence Frame %f", pUnit->dwSeqCurrentFramePrecise / 256.f);
    ImGui::BulletText("Seq next frame %f", pUnit->dwSeqFrame / 256.f);
    ImGui::EndDisabled();
}

const char* GetAlignmentString(D2C_UnitAlignment nAlignment)
{
    switch (nAlignment)
    {
    case UNIT_ALIGNMENT_EVIL: return "Evil";
    case UNIT_ALIGNMENT_NEUTRAL: return "Neutral";
    case UNIT_ALIGNMENT_GOOD: return "Good";
    case UNIT_NUM_ALIGNMENT: 
    case UNIT_ALIGNMENT_UNASSIGNED:
    default:
        return "Invalid alignment";
    }
}

void D2DebugUnitCommon(D2UnitStrc* pUnit)
{

    ImGui::Text("ClassId=%d", pUnit->dwClassId);
    ImGui::SameLine(); ImGui::Text("GUID=%d", pUnit->dwUnitId);
    ImGui::SameLine(); ImGui::Text(gActNames[pUnit->nAct]);
    D2CoordStrc tCoords;
    UNITS_GetCoords(pUnit, &tCoords);
    ImGui::SameLine(); ImGui::Text("(X,Y)=(%5d,%5d)", tCoords.nX, tCoords.nY);
    if (pUnit->dwUnitType == UNIT_PLAYER || pUnit->dwUnitType == UNIT_MONSTER)
    {
        ImGui::SameLine(); ImGui::Text("| %s", GetAlignmentString((D2C_UnitAlignment)STATLIST_GetUnitAlignment(pUnit)));
    }
    ImGui::SeparatorText("Animation");
    D2DebugUnitAnim(pUnit);
}

D2UnitStrc* GetFirstPlayerInList(D2GameStrc* pGame)
{
    for (D2UnitStrc* pUnitList : pGame->pUnitList[GAME_RemapUnitTypeToListIndex(UNIT_PLAYER)])
    {
        for (D2UnitStrc* pUnit = pUnitList;
            pUnit != nullptr;
            pUnit = pUnit->pListNext)
        {
            return pUnit;
        }
    }
    return nullptr;
}

const std::vector<char>& GetNOTFOUNDCharBuffer()
{
    static const char notFound[] = "NOT-FOUND";
    static std::vector<char> notFoundBuffer{notFound, notFound + sizeof(notFound)};
    return notFoundBuffer;
}

template<typename NAMEGETTER>
void D2ComboBox(const char* Title, int& selectedID, size_t count,NAMEGETTER&& NameGetter)
{

    static bool bWasComboOpen = false;
    if (ImGui::BeginCombo(Title, NameGetter(selectedID).data()))
    {
        ImGuiListClipper clipper;
        clipper.Begin(count);

        if (!bWasComboOpen)
        {
            bWasComboOpen = true;
            //This is needed on first frame for SetItemDefaultFocus to work
            clipper.ForceDisplayRangeByIndices(0, clipper.ItemsCount);
        }
        while (clipper.Step())
        {
            for (int id = clipper.DisplayStart; id < clipper.DisplayEnd; id++)
            {
                const bool bSelected = selectedID == id;
                if (ImGui::Selectable(NameGetter(id).data(), bSelected))
                    selectedID = id;
                if (bSelected)
                    ImGui::SetItemDefaultFocus();
            }
        }
        clipper.End();
        ImGui::EndCombo();
    }
    else
    {
        bWasComboOpen = false;
    }
}

void D2DebugUnitSpawner(D2GameStrc* pGame)
{
    if (ImGui::CollapsingHeader("UnitSpawner"))
    {
        D2CoordStrc tCoords{};
        if (D2UnitStrc* pPlayer = GetFirstPlayerInList(pGame))
        {
            UNITS_GetCoords(pPlayer, &tCoords);
            ImGui::Text("Spawn location (player) = (%d,%d)", tCoords.nX, tCoords.nY);


            auto GetSuperUniqueUTF8Name = [](int id)
            {
                if (D2SuperUniquesTxt* pSuperUniqueRecord = DATATBLS_GetSuperUniquesTxtRecord(id))
                {
                    return GetUTF8CharBufferFromStringIndex(pSuperUniqueRecord->wNameStr);
                }
                return GetNOTFOUNDCharBuffer();
            };

            static int currentSuperUniqueSelectionId = 0;
            D2ComboBox("SuperUnique", currentSuperUniqueSelectionId, DATATBLS_GetSuperUniquesTxtRecordCount(), GetSuperUniqueUTF8Name);
            ImGui::SameLine();
            if (ImGui::Button("Spawn##SuperUnique"))
            {
                if (D2UnitStrc* pSpawned = D2Game_SpawnSuperUnique_6FC6F690(pGame, pPlayer->pDynamicPath->pRoom, tCoords.nX, tCoords.nY, currentSuperUniqueSelectionId))
                {
                    // Register for debug view ?
                }
                else
                {
                    ImGui::OpenPopup("Spawn failed");
                }
            }


            static int currentNormalSelectionId = 0;
            auto GetNormalMonsterUTF8Name = [](int id)
            {
                if (D2MonStatsTxt* pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(id))
                {
                    return GetUTF8CharBufferFromStringIndex(pMonStatsTxtRecord->wNameStr);
                }
                return GetNOTFOUNDCharBuffer();
            };
            D2ComboBox("Normal", currentNormalSelectionId, DATATBLS_GetMonStatsTxtRecordCount(), GetNormalMonsterUTF8Name);
            ImGui::SameLine();
            if (ImGui::Button("Spawn##Normal"))
            {
                
                if (D2UnitStrc* pSpawned = D2Game_SpawnMonster_6FC69F10(pGame, pPlayer->pDynamicPath->pRoom, tCoords.nX, tCoords.nY, currentNormalSelectionId, MONMODE_NEUTRAL, 5, 0))
                {
                    // Register for debug view ?
                }
                else
                {
                    ImGui::OpenPopup("Spawn failed");
                }
            }



            bool bOpened_Unused = true;
            if (ImGui::BeginPopupModal("Spawn failed", &bOpened_Unused))
            {
                ImGui::Text("Spawning the monster failed. Possible issues:");
                ImGui::BulletText("Can only be spawned once per game.");
                ImGui::BulletText("Not engouh space left.");
                ImGui::BulletText("...");
                if (ImGui::Button("Close"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }

        }
    }
}

static bool bFreezeGame = false;
bool D2DebugGame(D2GameStrc* pGame)
{
    if(ImGui::Begin("Game"))
    {
        if (*pGame->szGameName)
            ImGui::Text("Game: '%s'", pGame->szGameName);
        if (*pGame->szGameDesc)
            ImGui::Text("Desc: '%s'", pGame->szGameDesc);
        if (*pGame->szGamePassword)
            ImGui::Text("Password: '%s'", pGame->szGamePassword);

        ImGui::Text(pGame->bExpansion ? "Expansion" : "Classic");
        ImGui::SameLine();
        ImGui::Text(pGame->dwGameType ? "(Ladder)" : "(Non-Ladder)");
        ImGui::Text("Difficulty:"); ImGui::SameLine();
        switch (pGame->nDifficulty)
        {
        case DIFFMODE_NORMAL:ImGui::Text("Normal"); break;
        case DIFFMODE_NIGHTMARE:ImGui::Text("Nightmare"); break;
        case DIFFMODE_HELL:ImGui::Text("Hell"); break;
        }
        ImGui::Text("Init seed: 0x%x", pGame->dwInitSeed);
        ImGui::Text("Frame %d", pGame->dwGameFrame);
        ImGui::SameLine();
        if(ImGui::SmallButton(bFreezeGame ? ICON_FA_PLAY : ICON_FA_PAUSE)) bFreezeGame = !bFreezeGame;

        ImGui::Separator();
        ImGui::Text("Last used GUID");
        ImGui::BulletText("Players: %d", pGame->dwLastUsedUnitGUID[UNIT_PLAYER]);
        ImGui::BulletText("Monsters: %d", pGame->dwLastUsedUnitGUID[UNIT_MONSTER]);
        ImGui::BulletText("Objects: %d", pGame->dwLastUsedUnitGUID[UNIT_OBJECT]);
        ImGui::BulletText("Missiles: %d", pGame->dwLastUsedUnitGUID[UNIT_MISSILE]);
        ImGui::BulletText("Items: %d", pGame->dwLastUsedUnitGUID[UNIT_ITEM]);
        ImGui::BulletText("Tiles: %d", pGame->dwLastUsedUnitGUID[UNIT_TILE]);

        D2DebugUnitSpawner(pGame);

        if (ImGui::CollapsingHeader("Players units"))
        {
            ImGui::Indent();
            DebutIterateUnitList(pGame, UNIT_PLAYER, [&](D2UnitStrc* pPlayer)
                {
                    AddDebugBreakButton();
                    ImGui::SameLine();
                    ImGui::SeparatorText(pPlayer->pPlayerData->szName);

                    D2DebugUnitCommon(pPlayer);
                    return true;
                }
            );
            ImGui::Unindent();
        }
        if (ImGui::CollapsingHeader("Monster units"))
        {
            ImGui::Indent();
            DebutIterateUnitList(pGame, UNIT_MONSTER, [](D2UnitStrc* pMonster)
                {
                    ImGui::Separator();
                    AddDebugBreakButton();
                    ImGui::SameLine();
                    D2DebugUnitCommon(pMonster);
                    return true;
                }
            );
            ImGui::Unindent();
        }
        if (ImGui::CollapsingHeader("Object units"))
        {
            ImGui::Indent();
            DebutIterateUnitList(pGame, UNIT_OBJECT, [](D2UnitStrc* pObject)
                {
                    ImGui::Separator();
                    AddDebugBreakButton();
                    ImGui::SameLine();
                    D2DebugUnitCommon(pObject);
                    return true;
                }
            );
            ImGui::Unindent();
        }
        if (ImGui::CollapsingHeader("Missile units"))
        {
            ImGui::Indent();
            DebutIterateUnitList(pGame, UNIT_MISSILE, [](D2UnitStrc* pMissile)
                {
                    ImGui::Separator();
                    AddDebugBreakButton();
                    ImGui::SameLine();
                    D2DebugUnitCommon(pMissile);
                    return true;
                }
            );
            ImGui::Unindent();
        }
        if (ImGui::CollapsingHeader("Item units"))
        {
            ImGui::Indent();
            DebutIterateUnitList(pGame, UNIT_ITEM, [](D2UnitStrc* pItem)
                {
                    ImGui::Separator();
                    AddDebugBreakButton();
                    ImGui::SameLine();
                    D2DebugUnitCommon(pItem);
                    return true;
                }
            );
            ImGui::Unindent();
        }

#if 0
        // No debugging for clients until we access the gClientListLock_6FD447D0 properly
        // This means knowing wether we use the one from the original game or D2Moo
        ImGui::Text("Clients");
        D2_LOCK(&gClientListLock_6FD447D0);

        for (D2ClientStrc* pCurrentClient = pGame->pClientList;
            pCurrentClient != nullptr;
            pCurrentClient = pCurrentClient->pNextByName)
        {
            ImGui::Text("Name:%16s", pCurrentClient->szName);
            ImGui::Text("Account:%16s", pCurrentClient->szAccount);
        }
        D2_UNLOCK(&gClientListLock_6FD447D0);
#endif
    }
    ImGui::End();

    return bFreezeGame;
}
