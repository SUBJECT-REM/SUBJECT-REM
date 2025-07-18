// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SRMouseInputComponent.generated.h"

class ASRSmoothRotateActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUBJECT_REM_API USRMouseInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USRMouseInputComponent();

	void OnClickMouse(bool bIsPressed);
	void OnMouseTurnAxis(FVector2D TrunAxis);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	ASRSmoothRotateActor* RotateActor;
	AActor* CheckClickHitActor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
