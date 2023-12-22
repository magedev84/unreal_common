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

	// 타일 인덱스 설정
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
	// 상대적인 위치를 얻는다
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


// 그리드 정보 갱신
void ACGridActor::ResetGridData()
{
	// 위치에 충돌체를 생성하여 충돌체크를 하고 그정보를 저장

	// 그리드 데이터의 리스트를 갱신
	GridData.ResetTileDataList();

	// 루프를 돈다
	FVector location = GetActorLocation();

	FVector extent(GridData.TileSize * 0.5f, GridData.TileSize * 0.5f, GridData.TileSize * 0.5f);

	// 루프를 돌면서 테스트용 박스를 생성
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

// 정보가 변경 되면(transform / 멤버 변수 포함) 호출
void ACGridActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// 출력
	DrawGridByCondition();
}



void ACGridActor::DrawGridByCondition()
{
	// 기존 디버깅 도형 삭제
	UKismetSystemLibrary::FlushPersistentDebugLines(this);

	// 출력하지 않는다면 리턴
	if (false == IsDrawGrid) {
		return;
	}

	// 그리드 정보 갱신
	GridData.UpdateGridInfo(GetActorLocation());

	// 위치 정보
	FVector location = GridData.Position;

	// 라인 출력 루프
	auto LoopDrawLine = [](UObject* inObject, FVector inLineStart, FVector inLineEnd, int32 inCount, FVector inIncrease)
		{
			for (int32 i = 0; i <= inCount; ++i)
			{
				UKismetSystemLibrary::DrawDebugLine(inObject, inLineStart, inLineEnd, FLinearColor::Red, 200000, 5);

				inLineStart += inIncrease;
				inLineEnd += inIncrease;
			}
		};

	// coloum을 그린다

	LoopDrawLine(this, location, location + FVector(0, GridData.MaxTileCountY * GridData.TileSize, 0), GridData.MaxTileCountX, FVector(GridData.TileSize, 0, 0));

	// row를 그린다
	LoopDrawLine(this, location, location + FVector(GridData.MaxTileCountX * GridData.TileSize, 0, 0), GridData.MaxTileCountY, FVector(0, GridData.TileSize, 0));

	// 타일정보 출력
	// 평면 
	//FPlane plane = UKismetMathLibrary::MakePlaneFromPointAndNormal(location, FVector::UpVector);


	FVector extent(GridData.TileSize * 0.5f, GridData.TileSize * 0.5f, 1);

	// 충돌 영역을 그린다
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


