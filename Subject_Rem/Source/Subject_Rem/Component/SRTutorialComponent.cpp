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
	if (TutorialManager.IsValid())
	{
		TutorialManager->NotifyObjectiveCompleted(ObjectiveTag);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s - USRTutirualComponent :Tutorial Manager nullptr "), *this->GetName());
	}
}

// Called when the game starts
void USRTutorialComponent::BeginPlay()
{
	Super::BeginPlay();

}

void USRTutorialComponent::InitializeComponent()
{
	Super::InitializeComponent();
	BindToManager();

}

void USRTutorialComponent::BindToManager()
{
	if (TutorialManager.IsValid()) return;

	if (UWorld* W = GetWorld())
	{
		if (AActor* ManagerActor = UGameplayStatics::GetActorOfClass(W, ASRTutorialManager::StaticClass()))
		{
			TutorialManager = Cast<ASRTutorialManager>(ManagerActor);
			UE_LOG(LogTemp, Warning, TEXT("TutoManager found : USRTutorialComponent"));

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TutoManager not found : USRTutorialComponent"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Getworld not vaild  : USRTutorialComponent"));

	}
	if (TutorialManager.IsValid())
	{
		TutorialManager->OnTutorialStartDelegate.AddDynamic(this, &ThisClass::HandleTutorialStarted);
		TutorialManager->OnTutorialCompleteDelegate.AddDynamic(this, &ThisClass::HandleTutorialCompleted);
		UE_LOG(LogTemp, Warning, TEXT("TutoManager bind : USRTutorialComponent"));

	}
}

void USRTutorialComponent::UnbindFromManager()
{
	TutorialManager->OnTutorialStartDelegate.RemoveDynamic(this, &ThisClass::HandleTutorialStarted);
	TutorialManager->OnTutorialCompleteDelegate.RemoveDynamic(this, &ThisClass::HandleTutorialCompleted);
	TutorialManager = nullptr;
}

void USRTutorialComponent::HideSelf()
{
	SetVisibility(false, true);
	UnbindFromManager();
}

void USRTutorialComponent::HandleTutorialStarted(FGameplayTag Tag)
{
	if (Tag == ExpectedTag)
	{
		UE_LOG(LogTemp, Warning, TEXT("WidgetComp SetVisibliry true : USRTutorialComponent"));
		// 즉시 표시
		SetVisibility(true, true);

		// 혹시 이전 타이머가 남아있다면 취소
		if (UWorld* W = GetWorld())
		{
			W->GetTimerManager().ClearTimer(HideTimerHandle);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("WidgetComp SetVisibliry Tag != ExpectedTag : USRTutorialComponent"));
}

void USRTutorialComponent::HandleTutorialCompleted(FGameplayTag Tag)
{
	//Widget제거
	if (UWorld* W = GetWorld())
	{
		W->GetTimerManager().ClearTimer(HideTimerHandle);
		W->GetTimerManager().SetTimer(
			HideTimerHandle, this, &ThisClass::HideSelf, TutorialWidgetHiddenDelay, false);
	}
}

