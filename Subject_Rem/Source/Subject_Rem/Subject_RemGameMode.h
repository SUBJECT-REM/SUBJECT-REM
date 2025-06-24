// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Subject_RemGameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class ASubject_RemGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	ASubject_RemGameMode();
};



