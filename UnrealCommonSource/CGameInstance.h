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

	// ƽ ó��
	virtual void Tick(float DeltaTime) override;

	// �������ϸ� ��� ���̵�
	virtual TStatId GetStatId() const override {
		return UObject::GetStatID();
	}


protected:
	// �����Ǵ� ����ý���
	UPROPERTY()
	TArray<class UCGameInstanceSubsystem*> SubsystemList;

	//------------------------------------------------------
	// ������ ���̺� �Ŵ���
	//------------------------------------------------------
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCDataTableManager> DataTableManagerClass;

	UPROPERTY()
	class UCDataTableManager* DataTableManager = nullptr;
};
