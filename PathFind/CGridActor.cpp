// Fill out your copyright notice in the Description page of Project Settings.


#include "unreal_common/PathFind/CGridActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "CGridActorManager.h"
#include "CGetter.h"


void FCGridData::UpdateGridInfo(const FVector& inPosition)
{
	Position = inPosition;

	MaxTileCountX = 0;
	MaxTileCountY = 0;

	if (0 < TileSize)
	{
		MaxTileCountX = GridSize.X / TileSize;
		MaxTileCountY = GridSize.Y / TileSize;
	}
}


void FCGridData::ResetTileDataList()
{
	const int32 count = MaxTileCountX * MaxTileCountY;
	TileDataList.SetNum(count);

	// Ÿ�� �ε��� ����
	if (0 < MaxTileCountX)
	{
		for (int32 i = 0; i < count; ++i)
		{
			int32 x = i % MaxTileCountX;
			int32 y = i / MaxTileCountX;

			TileDataList[i].Index2D.X = x;
			TileDataList[i].Index2D.Y = y;
		}
	}

}


FCTileData* FCGridData::GetTileData(int32 inX, int32 inY)
{
	if (inX < 0 || MaxTileCountX <= inX) {
		return nullptr;
	}

	if (inY < 0 || MaxTileCountY <= inY) {
		return nullptr;
	}


	const int32 index = (inY * MaxTileCountX) + inX;
	if (true == TileDataList.IsValidIndex(index)) {
		return &TileDataList[index];
	}

	return nullptr;
}

FCTileData* FCGridData::GetTileData(const FVector& inPos)
{
	// ������� ��ġ�� ��´�
	FVector relativePos = inPos - Position;

	int32 x = relativePos.X / TileSize;
	int32 y = relativePos.Y / TileSize;

	return GetTileData(x, y);
}



//--------------------------------------------------
// ACGridActor
//--------------------------------------------------
// Sets default values
ACGridActor::ACGridActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}


// �׸��� ���� ����
void ACGridActor::ResetGridData()
{
	// ��ġ�� �浹ü�� �����Ͽ� �浹üũ�� �ϰ� �������� ����

	// �׸��� �������� ����Ʈ�� ����
	GridData.ResetTileDataList();

	// ������ ����
	FVector location = GetActorLocation();

	FVector extent(GridData.TileSize * 0.5f, GridData.TileSize * 0.5f, GridData.TileSize * 0.5f);

	// ������ ���鼭 �׽�Ʈ�� �ڽ��� ����
	for (int32 x = 0; x < GridData.MaxTileCountX; ++x)
	{
		for (int32 y = 0; y < GridData.MaxTileCountY; ++y)
		{
			FVector center(
				location.X + x * GridData.TileSize + (GridData.TileSize * 0.5f),
				location.Y + y * GridData.TileSize + (GridData.TileSize * 0.5f),
				0);

			TArray<AActor*> ignoreList;

			FHitResult result;

			bool flag = UKismetSystemLibrary::BoxTraceSingle(
				this,
				center,
				center,
				extent,
				FRotator::ZeroRotator,
				UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1), false,
				ignoreList, EDrawDebugTrace::None, result, true);

			if (true == flag)
			{
				FString str = FString::Format(TEXT("X:{0}, Y:{1}"), { x, y });
				UKismetSystemLibrary::PrintString(GetWorld(), str, true, true, FLinearColor::Green, 2.0f);
			}

			if (FCTileData* tileData = GridData.GetTileData(x, y))
			{
				tileData->IsObstacle = flag;
			}
		}
	}
}

void ACGridActor::SetIsDrawGrid(bool inFlag)
{
	IsDrawGrid = inFlag;

	DrawGridByCondition();
}


// Called when the game starts or when spawned
void ACGridActor::BeginPlay()
{
	Super::BeginPlay();

	if (UCGridActorManager* actorManager = UCGetter::GetSubsystem<UCGridActorManager>(this)) {
		actorManager->AddGridActor(this);
	}
}

void ACGridActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UCGridActorManager* actorManager = UCGetter::GetSubsystem<UCGridActorManager>(this)) {
		actorManager->RemoveGridActor(this);
	}

	Super::EndPlay(EndPlayReason);
}

// ������ ���� �Ǹ�(transform / ��� ���� ����) ȣ��
void ACGridActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// ���
	DrawGridByCondition();
}



void ACGridActor::DrawGridByCondition()
{
	// ���� ����� ���� ����
	UKismetSystemLibrary::FlushPersistentDebugLines(this);

	// ������� �ʴ´ٸ� ����
	if (false == IsDrawGrid) {
		return;
	}

	// �׸��� ���� ����
	GridData.UpdateGridInfo(GetActorLocation());

	// ��ġ ����
	FVector location = GridData.Position;

	// ���� ��� ����
	auto LoopDrawLine = [](UObject* inObject, FVector inLineStart, FVector inLineEnd, int32 inCount, FVector inIncrease)
		{
			for (int32 i = 0; i <= inCount; ++i)
			{
				UKismetSystemLibrary::DrawDebugLine(inObject, inLineStart, inLineEnd, FLinearColor::Red, 200000, 5);

				inLineStart += inIncrease;
				inLineEnd += inIncrease;
			}
		};

	// coloum�� �׸���

	LoopDrawLine(this, location, location + FVector(0, GridData.MaxTileCountY * GridData.TileSize, 0), GridData.MaxTileCountX, FVector(GridData.TileSize, 0, 0));

	// row�� �׸���
	LoopDrawLine(this, location, location + FVector(GridData.MaxTileCountX * GridData.TileSize, 0, 0), GridData.MaxTileCountY, FVector(0, GridData.TileSize, 0));

	// Ÿ������ ���
	// ��� 
	//FPlane plane = UKismetMathLibrary::MakePlaneFromPointAndNormal(location, FVector::UpVector);


	FVector extent(GridData.TileSize * 0.5f, GridData.TileSize * 0.5f, 1);

	// �浹 ������ �׸���
	for (int32 x = 0; x < GridData.MaxTileCountX; ++x)
	{
		for (int32 y = 0; y < GridData.MaxTileCountY; ++y)
		{
			if (FCTileData* tileData = GridData.GetTileData(x, y))
			{
				if (true == tileData->IsObstacle)
				{
					FVector center(
						location.X + x * GridData.TileSize + (GridData.TileSize * 0.5f),
						location.Y + y * GridData.TileSize + (GridData.TileSize * 0.5f),
						location.Z);

					UKismetSystemLibrary::DrawDebugPlane(
						this,
						UKismetMathLibrary::MakePlaneFromPointAndNormal(center, FVector::UpVector),
						center,
						GridData.TileSize * 0.5f,
						FLinearColor::Blue, 999999);

					//this, center, extent, FLinearColor::Blue, FRotator::ZeroRotator, 999999, 5
				}
			}
		}
	}
}


