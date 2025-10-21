// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SRUserSettingSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SUBJECT_REM_API USRUserSettingSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

    UPROPERTY() 
    float SoundVol = 1.f;
    UPROPERTY()
    float MusicVol = 1.f;
    UPROPERTY() bool  bSoundOn = true;   // 전체 음소거 같은 토글
    UPROPERTY() bool  bAutoSave = true;   // 자동저장 토글 같이 보관

};
