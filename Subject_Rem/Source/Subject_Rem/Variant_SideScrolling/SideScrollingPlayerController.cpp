// Copyright Epic Games, Inc. All Rights Reserved.


#include "SideScrollingPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "SideScrollingCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Subsystem/SRInputLocalPlayerSubsystem.h"

void ASideScrollingPlayerController::CameraSettingToLevelSequance(bool bActive)
{

	if (bActive)
	{
		bAutoManageActiveCameraTarget = false;
		bIsInCinematic = true;
		SetCinematicMode(true, true, true, true, true);
	}
	else
	{
		bAutoManageActiveCameraTarget = true;
		bIsInCinematic = false;
		SetCinematicMode(false, false, true, true, true);
	}
}

void ASideScrollingPlayerController::SetupInputComponent()
{
	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (auto* InputSys = LP->GetSubsystem<USRInputLocalPlayerSubsystem>())
		{
			TArray<FIMCEntry> Defaults;
			for (UInputMappingContext* Ctx : DefaultMappingContexts)
			{
				if (Ctx)
				{
					Defaults.Emplace(Ctx, /*Priority=*/0);
				}
			}
			// 기본 세트를 등록하고 즉시 적용
			InputSys->SetDefaultContexts(Defaults);
		}
	}
}

void ASideScrollingPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// subscribe to the pawn's OnDestroyed delegate
	InPawn->OnDestroyed.AddDynamic(this, &ASideScrollingPlayerController::OnPawnDestroyed);
}

void ASideScrollingPlayerController::OnPawnDestroyed(AActor* DestroyedActor)
{
	// find the player start
	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), ActorList);

	if (ActorList.Num() > 0)
	{
		// spawn a character at the player start
		const FTransform SpawnTransform = ActorList[0]->GetActorTransform();

		if (ASideScrollingCharacter* RespawnedCharacter = GetWorld()->SpawnActor<ASideScrollingCharacter>(CharacterClass, SpawnTransform))
		{
			// possess the character
			Possess(RespawnedCharacter);
		}
	}
}

void ASideScrollingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly Mode;
	SetInputMode(Mode);
	bShowMouseCursor=false;

}
