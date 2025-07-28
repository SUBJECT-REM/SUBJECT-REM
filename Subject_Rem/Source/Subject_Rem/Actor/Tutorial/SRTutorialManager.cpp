// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Tutorial/SRTutorialManager.h"
#include "Kismet/GameplayStatics.h"
#include "SRGameplayTags.h"
#include "Subject_RemPlayerController.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ASRTutorialManager::ASRTutorialManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ASRTutorialManager::AddStateTag(FGameplayTag Tag)
{
	CurrentTags.AddTag(Tag);
}

void ASRTutorialManager::RemoveStateTag(FGameplayTag Tag)
{
	CurrentTags.RemoveTag(Tag);
}

// Called when the game starts or when spawned
void ASRTutorialManager::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	check(Controller);

	ASubject_RemPlayerController* SRController = Cast<ASubject_RemPlayerController>(Controller);
	check(SRController);
	EnableInput(Controller);

	// Add Input Mapping Contexts
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Controller->GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : SRController->GetDefaultMappingContexts())
		{
			if(CurrentContext)
				Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}

void ASRTutorialManager::Move()
{
	AddStateTag(SRGameplayTags::Tutorial_Objectives_Move);
}

FTutorialInfo ASRTutorialManager::FoundNextTutorialByTag(FGameplayTag Tag)
{
	for (FTutorialInfo Info : TutorialInfos)
	{
		if (Info.NextTutorial == Tag)
			return Info;
	}

	return FTutorialInfo();
}


