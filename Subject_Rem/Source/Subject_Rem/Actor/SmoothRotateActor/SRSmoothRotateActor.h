// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRSmoothRotateActor.generated.h"

UCLASS()
class SUBJECT_REM_API ASRSmoothRotateActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASRSmoothRotateActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
	UFUNCTION()
	void OnTouchBegin(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);
	UFUNCTION()
	void OnTouchEnd(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);


	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

public:	
	virtual void Tick(float DeltaTime) override;

};
