#include "GAME/Targets.h"


//D2Game.0x6FC40170
void __fastcall D2GAME_TARGETS_First_6FC40170(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3)
{
    if (!pGame || !pUnit || pUnit->dwUnitType != UNIT_PLAYER || pUnit->dwNodeIndex != 11)
    {
        return;
    }

    for (int32_t nNodeIndex = 0; nNodeIndex < 8; ++nNodeIndex)
    {
        if (!pGame->pTargetNodes[nNodeIndex])
        {
            if (pGame->nClients > 8)
            {
                return;
            }

            D2TargetNodeStrc* pTargetNode = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2TargetNodeStrc);
            if (!pTargetNode)
            {
                return;
            }

            memset(pTargetNode, 0x00, sizeof(D2TargetNodeStrc));

            pTargetNode->pUnit = pUnit;
            pTargetNode->unk0x04 = a3;

            pGame->pTargetNodes[nNodeIndex] = pTargetNode;
            pUnit->dwNodeIndex = nNodeIndex;
            return;
        }
    }
}

//D2Game.0x6FC401F0
void __fastcall D2GAME_UpdateSummonAI_6FC401F0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t nNodeIndex)
{
    if (!pGame || !pUnit || pUnit->dwNodeIndex != 11 || nNodeIndex >= 8 || (pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER))
    {
        return;
    }

    D2TargetNodeStrc* pTargetNode = pGame->pTargetNodes[nNodeIndex];
    if (!pTargetNode || !pTargetNode->pUnit)
    {
        return;
    }

    D2TargetNodeStrc* pNewTargetNode = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2TargetNodeStrc);
    if (!pNewTargetNode)
    {
        return;
    }

    memset(pNewTargetNode, 0x00, sizeof(D2TargetNodeStrc));

    pNewTargetNode->pUnit = pUnit;
    pNewTargetNode->unk0x04 = a3;

    pNewTargetNode->unk0x0C = pTargetNode;
    pNewTargetNode->pNext = pTargetNode->pNext;

    if (pTargetNode->pNext)
    {
        pTargetNode->pNext->unk0x0C = pNewTargetNode;
    }

    pTargetNode->pNext = pNewTargetNode;
    pUnit->dwNodeIndex = nNodeIndex;
}

//D2Game.0x6FC40280
void __fastcall sub_6FC40280(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t nNodeIndex)
{
    if (!pGame || !pUnit || pUnit->dwNodeIndex != 11 || nNodeIndex < 8 || nNodeIndex >= 10 || (pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER))
    {
        return;
    }

    D2TargetNodeStrc* pTargetNode = pGame->pTargetNodes[nNodeIndex];
    D2TargetNodeStrc* pNewTargetNode = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2TargetNodeStrc);
    if (!pNewTargetNode)
    {
        return;
    }

    memset(pNewTargetNode, 0x00, sizeof(D2TargetNodeStrc));

    pNewTargetNode->pUnit = pUnit;
    pNewTargetNode->unk0x04 = a3;

    if (pTargetNode)
    {
        pNewTargetNode->pNext = pTargetNode;
        pTargetNode->unk0x0C = pNewTargetNode;
    }

    pGame->pTargetNodes[nNodeIndex] = pNewTargetNode;
    pUnit->dwNodeIndex = nNodeIndex;
}

//D2Game.0x6FC40310
void __fastcall D2GAME_FreeUnitNode_6FC40310(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (!pGame || !pUnit || (pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER))
    {
        return;
    }

    const int32_t nNodeIndex = pUnit->dwNodeIndex;
    if (nNodeIndex == 11)
    {
        return;
    }

    D2TargetNodeStrc* pTargetNode = pGame->pTargetNodes[nNodeIndex];
    while (pTargetNode)
    {
        D2TargetNodeStrc* pNext = pTargetNode->pNext;
        if (pTargetNode->pUnit)
        {
            pTargetNode->pUnit->dwNodeIndex = 11;
        }

        D2_FREE_POOL(pGame->pMemoryPool, pTargetNode);
        pTargetNode = pNext;
    }
    pGame->pTargetNodes[nNodeIndex] = nullptr;
}

//D2Game.0x6FC40380
void __fastcall D2GAME_TARGETS_Last_6FC40380(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (!pGame || !pUnit || (pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER))
    {
        return;
    }

    const int32_t nNodeIndex = pUnit->dwNodeIndex;
    if (nNodeIndex == 11)
    {
        return;
    }

    D2TargetNodeStrc* pTargetNode = pGame->pTargetNodes[nNodeIndex];
    if (!pTargetNode)
    {
        return;
    }

    if (nNodeIndex >= 8)
    {
        while (pTargetNode->pUnit != pUnit)
        {
            pTargetNode = pTargetNode->pNext;
            if (!pTargetNode)
            {
                pUnit->dwNodeIndex = 11;
                return;
            }
        }

        if (pTargetNode == pGame->pTargetNodes[nNodeIndex])
        {
            pGame->pTargetNodes[nNodeIndex] = pTargetNode->pNext;

            if (pTargetNode->pNext)
            {
                pTargetNode->pNext->unk0x0C = nullptr;
            }

            D2_FREE_POOL(pGame->pMemoryPool, pTargetNode);
            pUnit->dwNodeIndex = 11;
            return;
        }
    }
    else
    {
        do
        {
            pTargetNode = pTargetNode->pNext;
        }
        while (pTargetNode && pTargetNode->pUnit != pUnit);

        if (!pTargetNode)
        {
            pUnit->dwNodeIndex = 11;
            return;
        }
    }

    pTargetNode->unk0x0C->pNext = pTargetNode->pNext;
    if (pTargetNode->pNext)
    {
        pTargetNode->pNext->unk0x0C = pTargetNode->unk0x0C;
    }

    D2_FREE_POOL(pGame->pMemoryPool, pTargetNode);
    pUnit->dwNodeIndex = 11;
}
