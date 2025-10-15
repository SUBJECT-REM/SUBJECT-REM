// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SRSaveGameSubsystem.generated.h"

/**
 * 
 */
class USRSaveGame;

UCLASS()
class SUBJECT_REM_API USRSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void LoadSaveGame(FString InSlotName = "");

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void CreateSaveSlot(const FString& SlotName);

protected:
	UFUNCTION()
	void OnLevelShown();

	/* Name of slot to save/load to disk. Filled by SaveGameSettings (can be overriden from GameMode's InitGame()) */
	FString CurrentSlotName = "TestSlot";

	UPROPERTY(Transient)
	TObjectPtr<USRSaveGame> CurrentSaveGame;
};
