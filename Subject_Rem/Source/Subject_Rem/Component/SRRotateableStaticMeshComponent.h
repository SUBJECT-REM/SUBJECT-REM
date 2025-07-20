// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "SRRotateableStaticMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class SUBJECT_REM_API USRRotateableStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
public:
	USRRotateableStaticMeshComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddRotationInput(FVector2D DeltaInput);

protected:
	UPROPERTY(EditAnywhere)
	float YawRotateSpeed = 1.0f;

	UPROPERTY(EditAnywhere)
	float PitchRotateSpeed = 1.0f;

	UPROPERTY(EditAnywhere)
	float RotationDamping = 6.0f;

	FVector2D RotateVelocity;
};
