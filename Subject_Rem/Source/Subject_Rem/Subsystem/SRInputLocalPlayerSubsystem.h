// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "SRInputLocalPlayerSubsystem.generated.h"

/**
 * 
 */
class UInputMappingContext;

USTRUCT()
struct FIMCEntry
{
    GENERATED_BODY()

public:
    UPROPERTY() TObjectPtr<UInputMappingContext> Context = nullptr;
    UPROPERTY() int32 Priority = 0;

    FIMCEntry() {}
    FIMCEntry(UInputMappingContext* InCtx, int32 InPri) : Context(InCtx), Priority(InPri) {}
    bool IsValid() const { return Context != nullptr; }
};


UCLASS()
class SUBJECT_REM_API USRInputLocalPlayerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	

public:
    //현재 적용중인 IMC에 대해 입력 제한을 설정함(캐싱한 IMC를 지웠다 다시 적용하는 방식으로 구현함)
    UFUNCTION(BlueprintCallable)
	void ApplyInputRestriction();

    //현재 적용중인 IMC에 대해 입력 제한을 해제함
    UFUNCTION(BlueprintCallable)
	void RemoveInputRestriction();

    //IMC를 교체함
    UFUNCTION(BlueprintCallable)
	void ReplaceContext(UInputMappingContext* ReplaceContext, int32 Priority);


private:

    //현재 적용중인 IMC을 캐싱한것
    FIMCEntry CachedIMC;
};
