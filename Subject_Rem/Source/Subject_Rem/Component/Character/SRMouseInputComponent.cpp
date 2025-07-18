// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Character/SRMouseInputComponent.h"
#include "Actor/SmoothRotateActor/SRSmoothRotateActor.h"

// Sets default values for this component's properties
USRMouseInputComponent::USRMouseInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USRMouseInputComponent::OnClickMouse(bool bIsPressed)
{
	UE_LOG(LogTemp, Log, TEXT("OnInput Mouse Click : %s"), bIsPressed ? TEXT("Press") : TEXT("Release"));
	if (bIsPressed)
	{
		AActor* ClickedActor = CheckClickHitActor();
		if (ClickedActor)
		{
			RotateActor = Cast<ASRSmoothRotateActor>(ClickedActor);
			if (RotateActor)
			{
				UE_LOG(LogTemp, Log, TEXT("RotateActor : %s"), *RotateActor->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("None RotateActor : %s"), *ClickedActor->GetName());
			}
		}
	}
	else 
	{
		RotateActor = nullptr;
	}
}

void USRMouseInputComponent::OnMouseTurnAxis(FVector2D TurnAxis)
{
	if (!RotateActor) return;

	RotateActor->AddRotationInput(TurnAxis);
}


// Called when the game starts
void USRMouseInputComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

AActor* USRMouseInputComponent::CheckClickHitActor()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return nullptr;

	FHitResult HitResult;
	if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			UE_LOG(LogTemp, Log, TEXT("Clicked Actor: %s"), *HitActor->GetName());
			return HitActor;
		}
	}
	return nullptr;
}


// Called every frame
void USRMouseInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

