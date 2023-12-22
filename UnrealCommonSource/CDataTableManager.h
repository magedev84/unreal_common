// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "CDataTableManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UCDataTableManager : public UObject
{
	GENERATED_BODY()

public:
	// 데이터 테이블을 얻는다
	static UDataTable* FindDataTable(UObject* inObj, UScriptStruct* inStruct);

	// 특정 타입의 데이터 테이블을 찾는다
	template<typename T>
	static UDataTable* FindDataTable(UObject* inObj)
	{
		UScriptStruct* target = T::StaticStruct();
		if (nullptr == target) {
			return nullptr;
		}

		return FindDataTable(inObj, target);
	}

	// 특정 row를 찾는다
	template<typename T>
	static T* FindRow(UObject* inObj, TFunction<bool(T* inRow)> inCheckFunc)
	{
		if (UDataTable* dataTable = UCDataTableManager::FindDataTable<T>(inObj))
		{
			auto rowMap = dataTable->GetRowMap();
			for (auto& pair : rowMap)
			{
				T* row = (T*)pair.Value;
				if (true == inCheckFunc(row)) {
					return row;
				}
			}
		}
		return nullptr;
	}

	template<typename T>
	static T* FindRow(UObject* inObj, int64 inID)
	{
		TCHAR buffer[30] = { 0, };
		TCString<TCHAR>::Sprintf(buffer, TEXT("%lld"), inID);

		return FindRow<T>(inObj, buffer);
	}

	template<typename T>
	static T* FindRow(UObject* inObj, const TCHAR* inID)
	{
		if (UDataTable* dataTable = FindDataTable<T>(inObj)) {
			return dataTable->FindRow<T>(inID, inID);
		}

		return nullptr;
	}

protected:	
	// 구조체 - 테이블 맵
	UPROPERTY(EditAnywhere)
	TMap<UScriptStruct*, TSoftObjectPtr<UDataTable>> DataTableMap;

	// 로드된 데이터 테이블 맵
	UPROPERTY()
	TMap<UScriptStruct*, UDataTable*> DataTableLoadedMap;
};
