// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/NoExportTypes.h"
#include "CGetter.generated.h"

/**
 * 
 */
UCLASS()
class UCGetter : public UObject
{
	GENERATED_BODY()
	
public:
	// 게임 인스턴스를 얻는다
	template <typename T>
	static T* GetGameInstance(const UObject* inObj)
	{
		return Cast<T>(UGameplayStatics::GetGameInstance(inObj));
	}

	// 서브 시스템을 얻는다
	template <typename T>
	static T* GetSubsystem(const UObject* inObj)
	{
		if (UGameInstance* gameInstance = UGameplayStatics::GetGameInstance(inObj)) {
			return gameInstance->GetSubsystem<T>();
		}
		return nullptr;
	}

	// 플레이어 컨트롤러를 얻는다
	template <typename T>
	static T* GetPlayerController(const UObject* inObj, int32 inPlayerIndex)
	{
		return Cast<T>(UGameplayStatics::GetPlayerController(inObj, inPlayerIndex));
	}


	template <typename T>
	static T* GetMyPlayerController(const UObject* inObj)
	{
		return Cast<T>(UGameplayStatics::GetPlayerController(inObj, 0));
	}
};
