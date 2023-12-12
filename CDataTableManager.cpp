// Fill out your copyright notice in the Description page of Project Settings.


#include "unreal_common/CDataTableManager.h"
#include "unreal_common/CGetter.h"
#include "unreal_common/CGameInstance.h"



UDataTable* UCDataTableManager::FindDataTable(UObject* inObj, UScriptStruct* inStruct)
{
	UCGameInstance* gameInstance = UCGetter::GetGameInstance<UCGameInstance>(inObj);
	if (nullptr == gameInstance) {
		return nullptr;
	}

	UCDataTableManager* dataTableManager = gameInstance->DataTableManager;
	if (nullptr == dataTableManager) {
		return nullptr;
	}

	UDataTable** dataTable = dataTableManager->DataTableLoadedMap.Find(inStruct);
	if (nullptr != dataTable) {
		return *dataTable;
	}

	// ���ٸ� ���̺� �ʿ��� ��ϵ� ������ ã�´�
	TSoftObjectPtr<UDataTable>* softObjectPtr = dataTableManager->DataTableMap.Find(inStruct);
	if (nullptr != softObjectPtr)
	{
		UDataTable* newDataTable = softObjectPtr->LoadSynchronous();
		if (nullptr != newDataTable)
		{
			// ���
			dataTableManager->DataTableLoadedMap.Add(inStruct, newDataTable);
			return newDataTable;
		}
	}

	return nullptr;
}