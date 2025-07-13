// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SRHUD.generated.h"

USTRUCT()
struct FUIControllerInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> UIControllerClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int32 ZOrder = 0;
};

UCLASS()
class SUBJECT_REM_API ASRHUD : public AHUD
{
	GENERATED_BODY()
private:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TArray<FUIControllerInfo> UIControllerInfos;
};
