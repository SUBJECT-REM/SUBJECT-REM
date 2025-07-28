// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRTutorialManager.generated.h"

UCLASS()
class SUBJECT_REM_API ASRTutorialManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASRTutorialManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
