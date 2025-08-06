// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SRTutorialComponent.h"
#include "Components/WidgetComponent.h"
#include "Actor/Tutorial/SRTutorialManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USRTutorialComponent::USRTutorialComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void USRTutorialComponent::NotifyTutorialObjective(FGameplayTag ObjectiveTag)
{
	if (TutorialManager)
	{
		if (TutorialCompletedDelaySeconds > 0)
		{
			FTimerHandle TutorialCompletedDelayHandle;
			PendingObjectiveTag = ObjectiveTag;
			GetWorld()->GetTimerManager().SetTimer(TutorialCompletedDelayHandle, this, &ThisClass::LateCompleteTutorialObjective, TutorialCompletedDelaySeconds, false);
		}
		else
		{
			TutorialManager->NotifyObjectiveCompleted(ObjectiveTag);
		}

	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s - USRTutirualComponent :Tutorial Manager nullptr "), *this->GetName());
	}
}

void USRTutorialComponent::SetTutorialWidgetComponent(UWidgetComponent* WidgetComp)
{
	OwnerTutorialWidgetComp = WidgetComp;
}


// Called when the game starts
void USRTutorialComponent::BeginPlay()
{
	Super::BeginPlay();

	TutorialManager = Cast<ASRTutorialManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ASRTutorialManager::StaticClass()));

	if (TutorialManager)
	{
		TutorialManager->OnTutorialStartDelegate.AddDynamic(this,&ThisClass::HandleTutorialStarted);
		TutorialManager->OnTutorialCompleteDelegate.AddDynamic(this, &ThisClass::HandleTutorialCompleted);
	}
}

void USRTutorialComponent::HandleTutorialStarted(FGameplayTag Tag)
{
	//Widget제거
	if (OwnerTutorialWidgetComp && Tag== ExpectedTag)
	{
		OwnerTutorialWidgetComp->SetVisibility(true);
	}
}

void USRTutorialComponent::HandleTutorialCompleted(FGameplayTag Tag)
{
	//Widget제거
	if (OwnerTutorialWidgetComp && Tag == ExpectedTag)
	{
		OwnerTutorialWidgetComp->SetVisibility(false);
	}
}

void USRTutorialComponent::LateCompleteTutorialObjective()
{
	if (TutorialManager && PendingObjectiveTag.IsValid())
	{
		TutorialManager->NotifyObjectiveCompleted(PendingObjectiveTag);
		PendingObjectiveTag = FGameplayTag(); // 초기화

	
	}

}

