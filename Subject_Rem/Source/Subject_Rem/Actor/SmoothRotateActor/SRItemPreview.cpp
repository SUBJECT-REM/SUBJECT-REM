// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SmoothRotateActor/SRItemPreview.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
ASRItemPreview::ASRItemPreview()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);
	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>("SceneCaptureComponent2D");
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
}

// Called when the game starts or when spawned
void ASRItemPreview::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASRItemPreview::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASRItemPreview::ReplaceStaticMesh(UStaticMesh* NewMesh)
{
	//메시 제거
	if (!NewMesh)
	{
		MeshComp->SetStaticMesh(nullptr);
		UE_LOG(LogTemp, Log, TEXT("Mesh cleared (set to nullptr)"));
		return;
	}

	MeshComp->SetStaticMesh(NewMesh);
	UE_LOG(LogTemp, Log, TEXT("Mesh replaced with: %s"), *NewMesh->GetName());
}
