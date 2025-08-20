// Copyright Epic Games, Inc. All Rights Reserved.


#include "Subject_RemPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Subsystem/SRInputLocalPlayerSubsystem.h"

void ASubject_RemPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Contexts
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
			UE_LOG(LogTemp, Warning, TEXT("SetDefaultContexts"));
			// 기본 세트를 등록하고 즉시 적용
			InputSys->SetDefaultContexts(Defaults);
		}
	}
}
