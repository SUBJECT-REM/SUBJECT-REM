// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SRSaveGameSubsystem.generated.h"

/**
 * 
 */
class USRSaveGame;
class USRUserSettingSaveGame;


USTRUCT(BlueprintType)
struct FSRUserSettingsRuntime
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float SoundVol = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float MusicVol = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool  bSoundOn = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool  bAutoSave = true;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUserSettingsApplied, const FSRUserSettingsRuntime&, Settings);

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

	UFUNCTION(BlueprintCallable, Category = "UserSettings")
	void SetSoundVol(float Vol);

	UFUNCTION(BlueprintCallable, Category = "UserSettings")
	void SetMusicVol(float Vol);

	UFUNCTION(BlueprintCallable, Category = "UserSettings")
	void SetSoundOn(bool IsOn);

	UFUNCTION(BlueprintCallable, Category = "UserSettings")
	void SetAutoSaveOnOff(bool IsOn);

		
	// 명시적 로드/세이브
	UFUNCTION(BlueprintCallable, Category = "UserSettings")
	void LoadUserSettings();

	UFUNCTION(BlueprintCallable, Category = "UserSettings")
	void SaveUserSettings();

	const TCHAR* SettingSaveSlotName = TEXT("UserSettings");

	UFUNCTION(BlueprintCallable, Category = "UserSettings")
	const FSRUserSettingsRuntime& GetUserSettings() const { return UserSettings; }

	UPROPERTY(BlueprintAssignable, Category = "UserSettings")
	FOnUserSettingsApplied OnUserSettingsApplied;
protected:
	UFUNCTION()
	void OnLevelShown();

	/* Name of slot to save/load to disk. Filled by SaveGameSettings (can be overriden from GameMode's InitGame()) */
	FString CurrentSlotName = "TestSlot";

	UPROPERTY(Transient)
	TObjectPtr<USRSaveGame> CurrentSaveGame;

	TObjectPtr<USRUserSettingSaveGame> UserSettingsSlot;

	void ApplyAudio();

	void ApplyAllSetting();

	FSRUserSettingsRuntime UserSettings;
};
