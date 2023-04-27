#include <imgui.h>
#include "D2Debugger.h"
#include <Game/Game.h>

#include "IconsFontAwesome6.h"

// Using a define so that we break inline
#define AddDebugBreakButton() do{ if (ImGui::Button(ICON_FA_HAMMER)) { __debugbreak(); }; } while(false)

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

        ImGui::SeparatorText("Players units");
        ImGui::Indent();
        for (int32_t i = 0; i < ARRAY_SIZE(pGame->pUnitList[UNIT_PLAYER]); ++i)
        {
            for (D2UnitStrc* pPlayer = pGame->pUnitList[UNIT_PLAYER][i];
                pPlayer != nullptr;
                pPlayer = pPlayer->pListNext)
            {
                AddDebugBreakButton();
                ImGui::SameLine();
                ImGui::SeparatorText(pPlayer->pPlayerData->szName);
                ImGui::Text("ClassId=%d", pPlayer->dwClassId);
                ImGui::Text("GUID=0x%x", pPlayer->dwUnitId);

                ImGui::Text("Act=0x%x", pPlayer->dwUnitId);

                ImGui::SeparatorText("Animation");
                //ImGui::Text("Animation");
                ImGui::BulletText("Mode           %d", pPlayer->dwAnimMode);
                ImGui::BulletText("Sequence Speed %d", pPlayer->dwAnimSequenceSpeed);
                ImGui::BulletText("Speed          %d", pPlayer->wAnimSpeed);
                ImGui::BulletText("FrameCount     %d", pPlayer->dwFrameCount);
                ImGui::BulletText("nActionFrame   %d", pPlayer->nActionFrame);
            }
        }
        ImGui::Unindent();

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
