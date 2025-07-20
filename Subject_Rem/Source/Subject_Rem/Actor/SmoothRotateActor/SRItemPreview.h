// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRItemPreview.generated.h"

class USceneComponent;
class USceneCaptureComponent2D;

UCLASS()
class SUBJECT_REM_API ASRItemPreview : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASRItemPreview();
	//어떤 StaticMesh로 랜더링할지 정함. nullptr을 넣으면 사라짐.
	void ReplaceStaticMesh(UStaticMesh* NewMesh);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere)
	USceneCaptureComponent2D* SceneCaptureComponent2D;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
