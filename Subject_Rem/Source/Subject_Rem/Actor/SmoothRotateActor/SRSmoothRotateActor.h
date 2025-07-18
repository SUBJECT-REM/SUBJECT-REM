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

	FORCEINLINE float GetYawRotateSpeed() const { return YawRotateSpeed; };
	FORCEINLINE float GetPitchRotateSpeed() const { return PitchRotateSpeed; };

	void ApplyRotate();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Rotate", meta = (PrivateAccess = "true"))
	float YawRotateSpeed = 3.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Rotate", meta = (PrivateAccess = "true"))
	float PitchRotateSpeed = 3.0f;

public:	
	virtual void Tick(float DeltaTime) override;

};
