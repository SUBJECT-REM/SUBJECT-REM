// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SmoothRotateActor/SRItemPreview.h"
#include "Component/SRRotateableStaticMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
ASRItemPreview::ASRItemPreview()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);
	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>("SceneCaptureComponent2D");
	RotateableMeshComp = CreateDefaultSubobject<USRRotateableStaticMeshComponent>(TEXT("RotateableMeshComp"));
}

// Called when the game starts or when spawned
void ASRItemPreview::BeginPlay()
{
	Super::BeginPlay();

	//RotateableMeshComp를 제외하고는 캡쳐하지 않도록 처리
	SceneCaptureComponent2D->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	SceneCaptureComponent2D->ShowOnlyComponents.Empty();
	SceneCaptureComponent2D->ShowOnlyComponents.Add(RotateableMeshComp);
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
		RotateableMeshComp->SetStaticMesh(nullptr);
		UE_LOG(LogTemp, Log, TEXT("Mesh cleared (set to nullptr)"));
		return;
	}

	RotateableMeshComp->SetStaticMesh(NewMesh);
	UE_LOG(LogTemp, Log, TEXT("Mesh replaced with: %s"), *NewMesh->GetName());
}
