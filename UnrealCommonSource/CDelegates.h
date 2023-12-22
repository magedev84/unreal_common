// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CDelegates.generated.h"

/**
 * 
 */
UCLASS()
class UCDelegates : public UObject
{
	GENERATED_BODY()
};



//--------------------------------------------------------------
// ½Ì±Û-Ä³½ºÆ®
//--------------------------------------------------------------
DECLARE_DELEGATE(FCDelegate_Param_Empty)
DECLARE_DELEGATE_OneParam(FCDelegate_Param_Int32, int32)