// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGridActor.generated.h"


//--------------------------------------------------
// 타일 정보
//--------------------------------------------------
USTRUCT()
struct FCTileData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FIntVector2 Index2D;
	
	UPROPERTY()
	bool IsObstacle = false;
};



//--------------------------------------------------
// 그리드 데이터
//--------------------------------------------------
USTRUCT()
struct FCGridData
{
	GENERATED_BODY()

public:
	// 타일정보 갱신
	void UpdateGridInfo(const FVector& inPosition);

	// 타일 정보 갱신
	void ResetTileDataList();

	// 타일 데이터를 얻는다
	FCTileData* GetTileData(int32 inX, int32 inY);
	FCTileData* GetTileData(const FVector& inPos);

public:
	// 위치
	UPROPERTY(VisibleAnywhere)
	FVector Position;

	// 그리드 사이즈
	UPROPERTY(EditAnywhere)
	FVector2D GridSize;

	// 타일 사이즈
	UPROPERTY(EditAnywhere)
	float TileSize;

	UPROPERTY(VisibleAnywhere)
	int32 MaxTileCountX;

	UPROPERTY(VisibleAnywhere)
	int32 MaxTileCountY;

	UPROPERTY(VisibleAnywhere)
	TArray<FCTileData> TileDataList;
};





UCLASS()
class PATH_FIND_TEST_API ACGridActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACGridActor();

	// 그리드 정보 갱신
	void ResetGridData();

	// 그리드를 출력할지
	void SetIsDrawGrid(bool inFlag);

	FCGridData* GetGridData() {
		return &GridData;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// 에디터에서 정보가 변경될때마다 갱신 처리
	virtual void OnConstruction(const FTransform& Transform) override;

	// 그리드 출력
	void DrawGridByCondition();

	

protected:
	// 그리드 데이터
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties_))
	FCGridData GridData;

	// 그리드 출력 여부
	bool IsDrawGrid = false;
};
