#include <imgui.h>
#include "D2Debugger.h"
#include <Game/Game.h>


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

    }
    ImGui::End();
}
