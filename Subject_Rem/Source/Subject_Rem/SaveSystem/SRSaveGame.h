// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SRSaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FName ActorName;

	UPROPERTY()
	TArray<uint8> ByteData;
};


/**
 * 
 */
UCLASS()
class SUBJECT_REM_API USRSaveGame : public USaveGame
{
	GENERATED_BODY()
	

	/* 레벨에서 저장된 액터 (현재는 특정 레벨을 지원하지 않고 데모 맵을 가정합니다) */
public:
	UPROPERTY()
	TMap<FName, FActorSaveData> SavedActorMap;
};
