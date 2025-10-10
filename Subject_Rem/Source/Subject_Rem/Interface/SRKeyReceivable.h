// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SRKeyReceivable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI,Blueprintable)
class USRKeyReceivable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SUBJECT_REM_API ISRKeyReceivable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    // 아이템 조건 충족 여부 (ID 또는 태그 기반)
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    bool CanUseKey(FName ItemId) const;   // 또는 FGameplayTag ItemTag

    // 실제로 아이템을 받아 상태 변경
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void UseKey(FName ItemId, AActor* InstigatorActor);
};
