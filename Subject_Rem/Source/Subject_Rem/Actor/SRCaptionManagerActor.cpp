// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SRCaptionManagerActor.h"
#include "SRCaptionManagerActor.h"

// Sets default values
ASRCaptionManagerActor::ASRCaptionManagerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ASRCaptionManagerActor::RequestCaptionShowing(const FName& RowName)
{
	CaptionRequestedDelegate.Broadcast(RowName);
}

// Called when the game starts or when spawned
void ASRCaptionManagerActor::BeginPlay()
{
	Super::BeginPlay();
	
}



