// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Tutorial/SRTutorialManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ASRTutorialManager::ASRTutorialManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASRTutorialManager::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	check(Controller);

	EnableInput(Controller);

	// Add Input Mapping Contexts
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Controller->GetLocalPlayer()))
	{
		
	}



}


