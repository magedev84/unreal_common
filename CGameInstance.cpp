// Fill out your copyright notice in the Description page of Project Settings.


#include "unreal_common/CGameInstance.h"
#include "unreal_common/CDataTableManager.h"
#include "CGameInstanceSubsystem.h"


void UCGameInstance::Init()
{	
	Super::Init();

	// ������ ���̺� ���� ��ü ����
	if (nullptr != DataTableManagerClass) {
		DataTableManager = NewObject<UCDataTableManager>(this, DataTableManagerClass);
	}
	
	// ������ ����ý����� ����
	SubsystemList = GetSubsystemArray<UCGameInstanceSubsystem>();
}

void UCGameInstance::Shutdown()
{
	// ����ý��� ����
	SubsystemList.Empty();

	Super::Shutdown();
}

void UCGameInstance::Tick(float DeltaTime)
{
	// ����ý��� ����
	{
		const int32 count = SubsystemList.Num();
		for (int32 i = 0; i < count; ++i)
		{
			SubsystemList[i]->Update(DeltaTime);
		}
	}
}