// Fill out your copyright notice in the Description page of Project Settings.


#include "unreal_common/PathFind/CGridActorManager.h"
#include "CGridActor.h"

#include "mage_astar.h"


void UCGridActorManager::AddGridActor(class ACGridActor* inGridActor)
{
	//GridActorSet.Emplace(inGridActor);
	GridActor = inGridActor;
}

void UCGridActorManager::RemoveGridActor(class ACGridActor* inGridActor)
{
	//GridActorSet.Remove(inGridActor);
	GridActor = nullptr;
}



bool UCGridActorManager::FindPath(TArray<FVector>& inList, const FVector& inStart, const FVector& inDest)
{
	inList.Empty();
	
	if (nullptr == GridActor) {
		return false;
	}

	FCGridData* gridData = GridActor->GetGridData();
	
	
	MAstar::MGrid grid;

	const int32 count = gridData->TileDataList.Num();
	grid.TileCount.Set(gridData->MaxTileCountX, gridData->MaxTileCountY);
	grid.TileList.resize(count);

	for (int32 i = 0; i < count; ++i)
	{
		grid.TileList[i].IsBlocked = gridData->TileDataList[i].IsObstacle;
	}

	std::vector<MVector2> posList;

	MAstar::MPathFinder pathFinder;
	
	pathFinder.FindPath(
		posList,
		MVector2(0, 0),
		gridData->TileSize,
		&grid,
		MVector2(inStart.X, inStart.Y),
		MVector2(inDest.X, inDest.Y), 10);

	for (int32 i = 0; i < posList.size(); ++i) {
		inList.Add(FVector(posList[i].X, posList[i].Y, 0));
	}

	return true;
}