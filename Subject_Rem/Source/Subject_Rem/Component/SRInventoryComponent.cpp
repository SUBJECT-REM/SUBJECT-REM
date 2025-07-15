// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SRInventoryComponent.h"

// Sets default values for this component's properties
USRInventoryComponent::USRInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void USRInventoryComponent::AddClueData(const FSRClueData& Data)
{
	ClueDatas.Add(Data);
	ChangeClueDatasDelegate.Broadcast(Data);
}



