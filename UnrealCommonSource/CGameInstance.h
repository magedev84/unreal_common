// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UCGameInstance : public UGameInstance, public FTickableGameObject
{
	GENERATED_BODY()
	
	friend class UCDataTableManager;

protected:
	virtual void Init() override;
	virtual void Shutdown() override;

	// 틱 처리
	virtual void Tick(float DeltaTime) override;

	// 프로파일링 통계 아이디
	virtual TStatId GetStatId() const override {
		return UObject::GetStatID();
	}


protected:
	// 관리되는 서브시스템
	UPROPERTY()
	TArray<class UCGameInstanceSubsystem*> SubsystemList;

	//------------------------------------------------------
	// 데이터 테이블 매니저
	//------------------------------------------------------
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCDataTableManager> DataTableManagerClass;

	UPROPERTY()
	class UCDataTableManager* DataTableManager = nullptr;
};
