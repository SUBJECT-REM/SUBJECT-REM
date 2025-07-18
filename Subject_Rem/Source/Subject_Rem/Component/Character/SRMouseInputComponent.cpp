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

	ApplyActorRotate(TurnAxis);
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

void USRMouseInputComponent::ApplyActorRotate(FVector2D TurnAxis)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	FVector ViewLocation;
	FRotator ViewRotation;
	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

	const float YawSpeed = RotateActor->GetYawRotateSpeed();
	const float PitchSpeed = RotateActor->GetPitchRotateSpeed();

	// 회전 기준 축은 ViewRotation 기준으로 고정
	FVector YawAxis = FVector::UpVector; // Yaw는 월드 Z축 그대로
	FVector PitchAxis = ViewRotation.RotateVector(FVector::RightVector); // Pitch는 '화면의 오른쪽' 방향

	// 회전량 → 쿼터니언
	FQuat YawQuat(YawAxis, FMath::DegreesToRadians(TurnAxis.X * YawSpeed));
	FQuat PitchQuat(PitchAxis, FMath::DegreesToRadians(-TurnAxis.Y * PitchSpeed)); // 마우스 상하는 반대

	// 누적 회전 적용
	FQuat NewQuat = PitchQuat * YawQuat * RotateActor->GetActorQuat();
	RotateActor->SetActorRotation(NewQuat);
}


// Called every frame
void USRMouseInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

