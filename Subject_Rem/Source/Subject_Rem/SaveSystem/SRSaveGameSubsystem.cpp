// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystem/SRSaveGameSubsystem.h"
#include "SaveSystem/SRSaveGame.h"
#include "SaveSystem/SRSaveableInterface.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine/LevelStreaming.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
//#include "Logging/StructeredLog.h"


void USRSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Warning, TEXT("USRSaveGameSubsystem : SaveGameSubSystem Init"));

	UWorld* World = GetGameInstance()->GetWorld();
	const TArray<ULevelStreaming*>& Levels = World->GetStreamingLevels();

	for (auto Level : Levels)
	{
		UE_LOG(LogTemp, Warning, TEXT("USRSaveGameSubsystem: Level Name :%s"), *Level->GetName());
		Level->OnLevelShown.AddDynamic(this, &ThisClass::OnLevelShown);
	}
}

void USRSaveGameSubsystem::Deinitialize()
{
	UWorld* World = GetGameInstance()->GetWorld();
	const TArray<ULevelStreaming*>& Levels = World->GetStreamingLevels();

	for (auto Level : Levels)
	{
		Level->OnLevelShown.RemoveDynamic(this, &ThisClass::OnLevelShown);
	}
}

void USRSaveGameSubsystem::WriteSaveGame()
{
	// Iterate the entire world of actors
	for (AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		// Only interested in our 'gameplay actors', skip actors that are being destroyed
		// Note: You might instead use a dedicated SavableObject interface for Actors you want to save instead of re-using GameplayInterface
		if (!IsValid(Actor) || !Actor->Implements<USRSaveableInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetFName();

		// Pass the array to fill with data from Actor
		FMemoryWriter MemWriter(ActorData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		// Find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;
		// Converts Actor's SaveGame UPROPERTIES into binary array
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActorMap.Add(Actor->GetFName(), ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, CurrentSlotName, 0);

}

void USRSaveGameSubsystem::LoadSaveGame(FString InSlotName)
{
	// Update slot name first if specified, otherwise keeps default name
	//SetSlotName(InSlotName);

	if (UGameplayStatics::DoesSaveGameExist(CurrentSlotName, 0))
	{
		CurrentSaveGame = Cast<USRSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("USRSaveGameSubsystem : Failed to load SaveGame Data."));

			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("USRSaveGameSubsystem: Loaded SaveGame Data."));

		// Iterate the entire world of actors
		for (AActor* Actor : TActorRange<AActor>(GetWorld()))
		{
			// Only interested in our 'gameplay actors'
			if (!Actor->Implements<USRSaveableInterface>())
			{
				continue;
			}

			if (FActorSaveData* FoundData = CurrentSaveGame->SavedActorMap.Find(Actor->GetFName()))
			{

				FMemoryReader MemReader(FoundData->ByteData);

				FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
				Ar.ArIsSaveGame = true;
				// Convert binary array back into actor's variables
				Actor->Serialize(Ar);

				ISRSaveableInterface::Execute_OnLoaded(Actor);
			}
		}

		//OnSaveGameLoaded.Broadcast(CurrentSaveGame);
	}
	else
	{
		CurrentSaveGame = CastChecked<USRSaveGame>(UGameplayStatics::CreateSaveGameObject(USRSaveGame::StaticClass()));

		UE_LOG(LogTemp, Warning, TEXT("USRSaveGameSubsystem : Created New SaveGame Data"));
	}
}

void USRSaveGameSubsystem::CreateSaveSlot(const FString& SlotName)
{
	CurrentSlotName = SlotName.IsEmpty() ? TEXT("TestSlot") : SlotName;
	CurrentSaveGame = Cast<USRSaveGame>(UGameplayStatics::CreateSaveGameObject(USRSaveGame::StaticClass()));
	UE_LOG(LogTemp, Warning, TEXT("[SaveTest] CreateNewSlot -> '%s' (no file yet)"), *CurrentSlotName);
}


void USRSaveGameSubsystem::OnLevelShown()
{

}
