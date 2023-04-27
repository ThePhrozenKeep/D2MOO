#include <imgui.h>
#include "D2Debugger.h"
#include <Game/Game.h>

#include "IconsFontAwesome6.h"

// Using a define so that we break inline
#define AddDebugBreakButton() do{ if (ImGui::Button(ICON_FA_HAMMER)) { __debugbreak(); }; } while(false)

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
void IterateUnits(D2GameStrc* pGame, const D2C_UnitTypes nUnitType, ItFunction&& itFunc)
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
            
            if (!itFunc(pUnit))
                stopIteration = true;

            if (nUnitGUIDFilter != D2UnitInvalidGUID)
                stopIteration = true;
        }
    }
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
    ImGui::BulletText("FrameCount     %d", pUnit->dwFrameCount);
    ImGui::BulletText("nActionFrame   %d", pUnit->nActionFrame);
    ImGui::BeginDisabled(pUnit->pAnimSeq == nullptr);
    ImGui::BulletText("Sequence Mode  %d", pUnit->dwSeqMode);
    ImGui::BulletText("Sequence Speed %d", pUnit->dwSeqSpeed);
    ImGui::BulletText("Sequence Frame %f", pUnit->dwSeqCurrentFrame / 256.f);
    ImGui::BulletText("Seq next frame %f", pUnit->dwSeqFrame / 256.f);
    ImGui::EndDisabled();
}

void D2DebugGame(D2GameStrc* pGame)
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

        ImGui::Separator();
        ImGui::Text("Spawned units");
        ImGui::BulletText("Players: %d", pGame->dwSpawnedUnits[UNIT_PLAYER]);
        ImGui::BulletText("Monsters: %d", pGame->dwSpawnedUnits[UNIT_MONSTER]);
        ImGui::BulletText("Objects: %d", pGame->dwSpawnedUnits[UNIT_OBJECT]);
        ImGui::BulletText("Missiles: %d", pGame->dwSpawnedUnits[UNIT_MISSILE]);
        ImGui::BulletText("Items: %d", pGame->dwSpawnedUnits[UNIT_ITEM]);
        ImGui::BulletText("Tiles: %d", pGame->dwSpawnedUnits[UNIT_TILE]);

        if (ImGui::CollapsingHeader("Players units"))
        {
            ImGui::Indent();
            IterateUnits(pGame, UNIT_PLAYER, [](D2UnitStrc* pPlayer)
                {
                    AddDebugBreakButton();
                    ImGui::SameLine();
                    ImGui::SeparatorText(pPlayer->pPlayerData->szName);

                    ImGui::Text("ClassId=%d", pPlayer->dwClassId);
                    ImGui::SameLine(); ImGui::Text("GUID=0x%x", pPlayer->dwUnitId);
                    ImGui::SameLine(); ImGui::Text(gActNames[pPlayer->nAct]);

                    ImGui::SeparatorText("Animation");
                    D2DebugUnitAnim(pPlayer);
                    return true;
                }
            );
            ImGui::Unindent();
        }
        if (ImGui::CollapsingHeader("Monster units"))
        {
            ImGui::Indent();
            IterateUnits(pGame, UNIT_MONSTER, [](D2UnitStrc* pMonster)
                {
                    ImGui::Separator();
                    AddDebugBreakButton();
                    ImGui::SameLine();

                    //ImGui::SeparatorText(pPlayer->pMonsterData->wszMonName);
                    ImGui::Text("ClassId=%d", pMonster->dwClassId);
                    ImGui::SameLine(); ImGui::Text("GUID=0x%x", pMonster->dwUnitId);
                    ImGui::SameLine(); ImGui::Text(gActNames[pMonster->nAct]);

                    ImGui::SeparatorText("Animation");
                    D2DebugUnitAnim(pMonster);
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
}
