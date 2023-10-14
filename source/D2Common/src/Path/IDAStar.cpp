#include "Path/Path.h"
#include "Path/IDAStar.h"
#include "Path/PathMisc.h"
#include "Path/FollowWall.h"
#include "Units/Units.h"

#include "D2Collision.h"
#include "D2Dungeon.h"
#include <cmath>

//1.10f: D2Common.0x6FDD1D60
//1.13c: D2Common.0x6FDDF508
static const D2CoordStrc aCoordOffsets[8] =
{
    {1,0},
    {1,1},
    {0,1},
    {-1,1},
    {-1,0},
    {-1,-1},
    {0,-1},
    {1,-1},
};

//1.10f: D2Common.0x6FDD1BE0
//1.13c: D2Common.0x6FDDB860
int32_t dword_6FDD1BE0[8][8] = {
    { 0, 1, 6, 3, 4, 5, 2, 7},
    { 0, 1, 6, 3, 1, 3, 6, 1},
    { 0, 1, 1, 1, 1, 1, 2, 7},
    { 1, 1, 1, 1, 1, 3, 6, 1},
    { 6, 4, 3, 6, 1, 3, 2, 7},
    { 7, 7, 7, 7, 7, 5, 2, 7},
    { 0, 7, 7, 7, 7, 5, 2, 7},
    { 0, 7, 2, 5, 7, 5, 2, 7},
};

//1.10f: Inlined
//1.13c: D2Common.0x6FDC0BB0
int __fastcall PATH_IdaStar_ComputePathWithRooms(D2DrlgCoordsStrc* pRoomCoords, D2PathInfoStrc* pPathInfo)
{
    D2_ASSERT(pRoomCoords->nSubtileHeight * pRoomCoords->nSubtileWidth <= IDASTAR_MAXPROOM);
    D2PathIDAStarContextStrc tContext;
    const int nAreaMargin = 6;
    const int nMaxNodes = (pRoomCoords->nSubtileHeight + nAreaMargin) * (pRoomCoords->nSubtileWidth + nAreaMargin);
    memset(tContext.aCoordData, 0, sizeof(int32_t) * nMaxNodes); // Note, may overflow since assert does not take margin into account.
    tContext.nCoord[0].Y = pRoomCoords->nSubtileY;
    tContext.nCoord[0].X = pRoomCoords->nSubtileX;
    tContext.nCoord[1].X = pRoomCoords->nSubtileX + pRoomCoords->nSubtileWidth;
    tContext.nCoord[1].Y = pRoomCoords->nSubtileY + tContext.nCoord[2].Y;
    tContext.nCoord[2].X = pRoomCoords->nSubtileWidth;
    tContext.nCoord[2].Y = pRoomCoords->nSubtileHeight;
    tContext.nStride = pRoomCoords->nSubtileWidth + 6;
    tContext.nYOffset = -(pRoomCoords->nSubtileY - 3);
    tContext.nXOffset = -(pRoomCoords->nSubtileX - 3);
    tContext.bRandomDirection = FALSE;

    if (pPathInfo->nPathType == PATHTYPE_MISSILE_STREAM)
    {
        tContext.bRandomDirection = 1;
        tContext.pSeed = &pPathInfo->pDynamicPath->pUnit->pSeed;
    }
    D2PathPointStrc tStartCoord = pPathInfo->tStartCoord;
    D2PathPointStrc tTargetCoord = pPathInfo->tTargetCoord;

    D2PathIDAStarNodeStrc tStartNode;
    tStartNode.nBestDistanceFromStart = 0;
    tStartNode.nHeuristicDistanceToTarget = PATH_FoWall_Heuristic(tStartCoord, tTargetCoord);
    tStartNode.nFScore = tStartNode.nHeuristicDistanceToTarget;
    tStartNode.tCoord = tStartCoord;
    tStartNode.nEvaluationsCount = -3;
    tStartNode.pNeighborsSequence = dword_6FDD1BE0[0];
    tStartNode.pBestChild = 0;
    tStartNode.pParent = 0;
    tStartNode.nNextNeighborIndex = sub_6FDAB770(tStartCoord, tTargetCoord) & 7;

    memset(&tContext.aNodesStorage[0], 0, sizeof(tContext.aNodesStorage[0]));
    tContext.nNodesCount = 1;
    tContext.pCurrentNode = &tContext.aNodesStorage[0];
    
    int16_t nMaxFScore;
    int16_t nFScoreCutoff;
    switch (pPathInfo->nPathType)
    {
    case PATHTYPE_MISSILE_STREAM:
        nFScoreCutoff = tStartNode.nHeuristicDistanceToTarget + tStartNode.nHeuristicDistanceToTarget / 2;
        nMaxFScore = pPathInfo->nMinimumFScoreToEvaluate;
        if (nFScoreCutoff > nMaxFScore)
        {
            nMaxFScore = nFScoreCutoff;
        }
        break;
    case PATHTYPE_IDASTAR:
        nFScoreCutoff = tStartNode.nHeuristicDistanceToTarget;
        nMaxFScore = pPathInfo->nMinimumFScoreToEvaluate;
        if (nMaxFScore > pPathInfo->nMinimumFScoreToEvaluate)
        {
            nMaxFScore = tStartNode.nHeuristicDistanceToTarget;
        }
        break;
    default:
        nFScoreCutoff = tStartNode.nHeuristicDistanceToTarget;
        nMaxFScore = tStartNode.nHeuristicDistanceToTarget;
        break;
    }
    D2PathIDAStarNodeStrc* pValidPathNode = nullptr;
    do
    {
        *tContext.pCurrentNode = tStartNode;

        pValidPathNode = PATH_IDAStar_VisitNodes(&tContext, nFScoreCutoff, pPathInfo);
        nFScoreCutoff += 5;
        if (!pValidPathNode && tContext.nNodesCount == ARRAY_SIZE(tContext.aNodesStorage))
            break;
        tContext.nNodesCount = 1;
        if (pValidPathNode)
            break;
    } while (nFScoreCutoff < nMaxFScore);

    int nPathPoints = 0;
    if (pValidPathNode)
    {
        nPathPoints = PATH_IDAStar_FlushNodeToDynamicPath(pValidPathNode, pPathInfo);
        if (nPathPoints >= D2DynamicPathStrc::MAXPATHLEN)
            nPathPoints = 0;
    }
    return nPathPoints;
}

//1.10f: D2Common.0x6FDA7970
//1.13c: D2Common.0x6FDC0E40
int __fastcall PATH_IdaStar_6FDA7970(D2PathInfoStrc* pPathInfo)
{
	pPathInfo->field_14 *= 2;
	D2DrlgCoordsStrc tStartRoomCoords;
	DUNGEON_GetRoomCoordinates(pPathInfo->pStartRoom, &tStartRoomCoords);
	D2DrlgCoordsStrc tPathRoomsAABB = tStartRoomCoords;

	int nAABBHeight = 0;
	int nAABBWidth = 0;

	if (!pPathInfo->pTargetRoom || pPathInfo->pStartRoom == pPathInfo->pTargetRoom)
	{
		nAABBHeight = tPathRoomsAABB.nSubtileHeight;
		nAABBWidth = tPathRoomsAABB.nSubtileWidth;
	}
	else
	{
		D2DrlgCoordsStrc tTargetRoomCoords;
		DUNGEON_GetRoomCoordinates(pPathInfo->pTargetRoom, &tTargetRoomCoords);
		if (tTargetRoomCoords.nSubtileX >= tStartRoomCoords.nSubtileX)
		{
			tPathRoomsAABB.nSubtileX = tStartRoomCoords.nSubtileX;
			nAABBWidth = tTargetRoomCoords.nSubtileX + tTargetRoomCoords.nSubtileWidth - tStartRoomCoords.nSubtileX;
		}
		else
		{
			tPathRoomsAABB.nSubtileX = tTargetRoomCoords.nSubtileX;
			nAABBWidth = tStartRoomCoords.nSubtileX + tStartRoomCoords.nSubtileWidth - tTargetRoomCoords.nSubtileX;
		}
		tPathRoomsAABB.nSubtileWidth = nAABBWidth;
		if (tTargetRoomCoords.nSubtileY >= tStartRoomCoords.nSubtileY)
		{
			nAABBHeight = tTargetRoomCoords.nSubtileY + tTargetRoomCoords.nSubtileHeight - tStartRoomCoords.nSubtileY;
			tPathRoomsAABB.nSubtileY = tStartRoomCoords.nSubtileY;
		}
		else
		{
			nAABBHeight = tStartRoomCoords.nSubtileY + tStartRoomCoords.nSubtileHeight - tTargetRoomCoords.nSubtileY;
			tPathRoomsAABB.nSubtileY = tTargetRoomCoords.nSubtileY;
		}
		tPathRoomsAABB.nSubtileHeight = nAABBHeight;
	}

	// allow pathing at a distance of +/-10 subtiles if AABB is the same size as the start room
	if (nAABBWidth == tStartRoomCoords.nSubtileWidth)
	{
		tPathRoomsAABB.nSubtileX -= 10;
		tPathRoomsAABB.nSubtileWidth = nAABBWidth + 20;
	}
	if (nAABBHeight == tStartRoomCoords.nSubtileHeight)
	{
		nAABBHeight += 20;
		tPathRoomsAABB.nSubtileY -= 10;
		tPathRoomsAABB.nSubtileHeight = nAABBHeight;
	}
	return PATH_IdaStar_ComputePathWithRooms(&tPathRoomsAABB, pPathInfo);
}

//1.10f: Inlined
//1.13c: Inlined
D2PathIDAStarNodeStrc* __fastcall PATH_IDAStar_GetNewNode(D2PathIDAStarContextStrc* pContext)
{
    if (pContext->nNodesCount == ARRAY_SIZE(pContext->aNodesStorage))
    {
        return nullptr;
    }
    D2PathIDAStarNodeStrc* pNewNode = &pContext->aNodesStorage[pContext->nNodesCount++];
    memset(pNewNode, 0, sizeof(*pNewNode));
    return pNewNode;
}

//1.10f: D2Common.0x6FDA7D40
//1.13c: D2Common.0x6FDC08F0
D2PathIDAStarNodeStrc* __fastcall PATH_IDAStar_VisitNodes(D2PathIDAStarContextStrc* pContext, int nFScoreCutoff, D2PathInfoStrc* pPathInfo)
{
    D2PathIDAStarNodeStrc* pCurrentNode = pContext->pCurrentNode;
    int nIterations = 0;
    while (pCurrentNode->tCoord != pPathInfo->tTargetCoord)
    {
        if (++nIterations > 10000)
        {
            return nullptr;
        }

        if (pCurrentNode->tCoord.X < pContext->nCoord[0].X || pCurrentNode->tCoord.X > pContext->nCoord[1].X
         || pCurrentNode->tCoord.Y < pContext->nCoord[0].Y || pCurrentNode->tCoord.Y > pContext->nCoord[1].Y)
        {
            break;
        }

        D2PathPointStrc tNeighborCoords;
        tNeighborCoords.X = pCurrentNode->tCoord.X + aCoordOffsets[pCurrentNode->nNextNeighborIndex].nX;
        tNeighborCoords.Y = pCurrentNode->tCoord.Y + aCoordOffsets[pCurrentNode->nNextNeighborIndex].nY;

        const int nDataIndex = tNeighborCoords.X + pContext->nXOffset + pContext->nStride * (tNeighborCoords.Y + pContext->nYOffset);
        int* pNeighborBestDistanceToStart = &pContext->aCoordData[nDataIndex];
        
        bool bShouldEvaluateNextNeighbor = true;
        bool bMayEvaluateNode = true;
        if (*pNeighborBestDistanceToStart == 0)
        {
            if (COLLISION_CheckAnyCollisionWithPattern(
                pPathInfo->pStartRoom,
                tNeighborCoords.X, tNeighborCoords.Y,
                pPathInfo->nCollisionPattern, pPathInfo->nCollisionMask))
            {
                // This will be lower than any other distance, since distance to a neighbor is 2 or 3
                // Thus it should never be considered again.
                *pNeighborBestDistanceToStart = 1;
                bMayEvaluateNode = false;
            }
        }

        if(bMayEvaluateNode)
        {
            const int16_t nDistanceBetweenPoints = PATH_FoWall_HeuristicForNeighbor(pCurrentNode->tCoord, tNeighborCoords);
            const int16_t nNewDistanceFromStart = nDistanceBetweenPoints + pCurrentNode->nBestDistanceFromStart;
            if (*pNeighborBestDistanceToStart == 0 || (unsigned int)nNewDistanceFromStart >= *pNeighborBestDistanceToStart )
            {
                *pNeighborBestDistanceToStart = nNewDistanceFromStart;

                const int16_t nHeuristicDistanceToTarget = PATH_FoWall_Heuristic(pPathInfo->tTargetCoord, tNeighborCoords);
                const int16_t nNewNodeFSCore = nHeuristicDistanceToTarget + nNewDistanceFromStart;
                if (nNewNodeFSCore <= nFScoreCutoff)
                {
                    if (!pCurrentNode->pBestChild)
                    {
                        D2PathIDAStarNodeStrc* pNewSubposition = PATH_IDAStar_GetNewNode(pContext);
                        pCurrentNode->pBestChild = pNewSubposition;
                        if (!pNewSubposition)
                        {
                            return nullptr;
                        }
                        pNewSubposition->pParent = pCurrentNode;
                    }
                    pCurrentNode = pCurrentNode->pBestChild;
                    pCurrentNode->tCoord = tNeighborCoords;
                    pCurrentNode->nBestDistanceFromStart = nNewDistanceFromStart;
                    pCurrentNode->nFScore = nNewNodeFSCore;
                    pCurrentNode->nHeuristicDistanceToTarget = nHeuristicDistanceToTarget;
                    pCurrentNode->nEvaluationsCount = 0;
                    pCurrentNode->pNeighborsSequence = dword_6FDD1BE0[(sub_6FDAB770(tNeighborCoords, pPathInfo->tTargetCoord) - pCurrentNode->pParent->nNextNeighborIndex) & 7];
                    PATH_IDAStar_GetNextNeighborIndex(pCurrentNode, pContext);

                    if ((__int16)pCurrentNode->nHeuristicDistanceToTarget < pPathInfo->field_14)
                    {
                        return pCurrentNode;
                    }
                    bShouldEvaluateNextNeighbor = false;
                }
            }
        }

        if (bShouldEvaluateNextNeighbor)
        {
            pCurrentNode = sub_6FDC0840(pCurrentNode, pContext);
            if (!pCurrentNode)
            {
                return nullptr;
            }
        }
    }
    return pCurrentNode;
}

// 1.10f: 0x6FDD1CE0
// 1.13c: 0x6FDDB960
int dword_6FDD1CE0[32] = {
	-2, -1,  0,  1,
	 2, -2, -1,  0,
	 1,  2, -2, -1,
	 0,  1,  2, -2,
	-1,  0,  1,  2,
	-2, -1,  0,  1,
	 2, -2, -1,  0,
	 1,  2, -1,  1
};

//1.10c: D2Common.0x6FDA81C0
//1.13f: D2Common.0x6FDC07E0
void __fastcall PATH_IDAStar_GetNextNeighborIndex(D2PathIDAStarNodeStrc* pNode, D2PathIDAStarContextStrc* pContext)
{
	if (pContext->bRandomDirection)
	{
		const uint64_t nRand = SEED_RollRandomNumber(pContext->pSeed);
		pNode->nNextNeighborIndex = ((unsigned __int8)pNode->nNextNeighborIndex
			+ (unsigned __int8)*pNode->pNeighborsSequence
			+ (unsigned __int8)dword_6FDD1CE0[nRand & 0x1F]) & 7;
	}
	else
	{
		pNode->nNextNeighborIndex = (pNode->nNextNeighborIndex + *pNode->pNeighborsSequence) & 7;
	}
}

//1.10f: Inlined
//1.13c: D2Common.0x6FDC0840
D2PathIDAStarNodeStrc* __fastcall sub_6FDC0840(D2PathIDAStarNodeStrc* pNode, D2PathIDAStarContextStrc* pContext)
{
    if (pNode->nEvaluationsCount < 4)
    {
        pNode->pNeighborsSequence++;
        PATH_IDAStar_GetNextNeighborIndex(pNode, pContext);
    }

    pNode->nEvaluationsCount++;
    if (pNode->nEvaluationsCount == 5)
    {
        while (pNode != pContext->pCurrentNode)
        {
            pNode = pNode->pParent;
            pNode->pNeighborsSequence++;

            PATH_IDAStar_GetNextNeighborIndex(pNode, pContext);

            if ((++pNode->nEvaluationsCount) != 5)
            {
                return pNode;
            }
        }
        return 0;
    }

    return pNode;
}


//1.10f: Inlined
//1.13c: D2Common.0x6FDC0650
signed int __fastcall PATH_IDAStar_FlushNodeToDynamicPath(D2PathIDAStarNodeStrc* pNode, D2PathInfoStrc* pPathInfo)
{
    if (!pNode)
    {
        return 0;
    }

    int nbPoints = 0;
    D2PathPointStrc aTempPathPoints[78];
    // Assumes all points are conex, so we can't have a delta of -2, hence used for init
    int prevDeltaX = -2;
    int prevDeltaY = -2;
    while (pNode && pNode->pParent && nbPoints < D2DynamicPathStrc::MAXPATHLEN)
    {
        D2PathIDAStarNodeStrc* pNextPoint = pNode->pParent;
        const int deltaX = pNode->tCoord.X - pNextPoint->tCoord.X;
        const int deltaY = pNode->tCoord.Y - pNextPoint->tCoord.Y;
        // If the direction doesn't change, then ignore the point
        if (deltaX != prevDeltaX || deltaY != prevDeltaY)
        {
            ++nbPoints;
            // Store path in reverse order
            aTempPathPoints[D2DynamicPathStrc::MAXPATHLEN - nbPoints] = pNode->tCoord;
            prevDeltaX = deltaX;
            prevDeltaY = deltaY;
        }
        pNode = pNextPoint;
    }

    if (nbPoints <= 1 || nbPoints >= D2DynamicPathStrc::MAXPATHLEN)
    {
        return 0;
    }

    memcpy(pPathInfo->pDynamicPath->PathPoints, &aTempPathPoints[D2DynamicPathStrc::MAXPATHLEN - nbPoints], sizeof(D2PathPointStrc) * nbPoints);
    return nbPoints;
}