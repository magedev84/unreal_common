// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UCGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// 갱신 처리
	virtual void Update(float inDelta) {}
};
