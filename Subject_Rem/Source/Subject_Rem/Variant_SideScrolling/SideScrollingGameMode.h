// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SideScrollingGameMode.generated.h"

class USideScrollingUI;

/**
 *  Simple Side Scrolling Game Mode
 *  Spawns and manages the game UI
 *  Counts pickups collected by the player
 */
UCLASS(abstract)
class ASideScrollingGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	/** Initialization */
	virtual void BeginPlay() override;

public:

};
