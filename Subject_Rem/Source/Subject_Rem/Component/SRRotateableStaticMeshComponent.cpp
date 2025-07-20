// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SRRotateableStaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

USRRotateableStaticMeshComponent::USRRotateableStaticMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USRRotateableStaticMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (RotateVelocity.IsNearlyZero()) return;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController) return;

	FVector ViewLocation;
	FRotator ViewRotation;
	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

	FVector YawAxis = FVector::UpVector;
	FVector PitchAxis =  ViewRotation.RotateVector(FVector::RightVector);

	FQuat YawQuat(YawAxis, FMath::DegreesToRadians(-RotateVelocity.X * YawRotateSpeed * DeltaTime));
	FQuat PitchQuat(PitchAxis, FMath::DegreesToRadians(-RotateVelocity.Y * PitchRotateSpeed * DeltaTime));

	FQuat NewQuat = PitchQuat * YawQuat * GetComponentQuat();
	SetWorldRotation(NewQuat);

	RotateVelocity = FMath::Vector2DInterpTo(RotateVelocity, FVector2D::ZeroVector, DeltaTime, RotationDamping);
}

void USRRotateableStaticMeshComponent::AddRotationInput(FVector2D DeltaInput)
{
	RotateVelocity += DeltaInput * 5.0f;
}
