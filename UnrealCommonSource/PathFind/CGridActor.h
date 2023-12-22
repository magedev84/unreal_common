// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGridActor.generated.h"


//--------------------------------------------------
// Ÿ�� ����
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
// �׸��� ������
//--------------------------------------------------
USTRUCT()
struct FCGridData
{
	GENERATED_BODY()

public:
	// Ÿ������ ����
	void UpdateGridInfo(const FVector& inPosition);

	// Ÿ�� ���� ����
	void ResetTileDataList();

	// Ÿ�� �����͸� ��´�
	FCTileData* GetTileData(int32 inX, int32 inY);
	FCTileData* GetTileData(const FVector& inPos);

public:
	// ��ġ
	UPROPERTY(VisibleAnywhere)
	FVector Position;

	// �׸��� ������
	UPROPERTY(EditAnywhere)
	FVector2D GridSize;

	// Ÿ�� ������
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

	// �׸��� ���� ����
	void ResetGridData();

	// �׸��带 �������
	void SetIsDrawGrid(bool inFlag);

	FCGridData* GetGridData() {
		return &GridData;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// �����Ϳ��� ������ ����ɶ����� ���� ó��
	virtual void OnConstruction(const FTransform& Transform) override;

	// �׸��� ���
	void DrawGridByCondition();

	

protected:
	// �׸��� ������
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties_))
	FCGridData GridData;

	// �׸��� ��� ����
	bool IsDrawGrid = false;
};
