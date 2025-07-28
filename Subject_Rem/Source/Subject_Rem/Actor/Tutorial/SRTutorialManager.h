// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "SRTutorialManager.generated.h"


USTRUCT()
struct FTutorialInfo
{
	GENERATED_BODY()

	FGameplayTag ID;

	FGameplayTag ObjectivesTag;

	FGameplayTag NextTutorial;
};



UCLASS()
class SUBJECT_REM_API ASRTutorialManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASRTutorialManager();

	void AddStateTag(FGameplayTag Tag);
	void RemoveStateTag(FGameplayTag Tag);

	FGameplayTagContainer CurrentTags;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Move();

private:
	FTutorialInfo FoundNextTutorialByTag(FGameplayTag Tag);


	UPROPERTY(EditDefaultsOnly)
	TArray<FTutorialInfo> TutorialInfos;
};
