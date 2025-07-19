// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SmoothRotateActor/SRSmoothRotateActor.h"

// Sets default values
ASRSmoothRotateActor::ASRSmoothRotateActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	// 클릭 이벤트 활성화
	MeshComp->SetMobility(EComponentMobility::Movable);
	MeshComp->SetGenerateOverlapEvents(true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCollisionObjectType(ECC_WorldDynamic);
	//MeshComp->SetCollisionResponseToAllChannels(ECR_Block);
	MeshComp->SetEnableGravity(false);
	MeshComp->SetNotifyRigidBodyCollision(true);
	MeshComp->SetSimulatePhysics(false);
	MeshComp->bSelectable = true;

	MeshComp->SetMobility(EComponentMobility::Static);
}

// Called when the game starts or when spawned
void ASRSmoothRotateActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASRSmoothRotateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RotateVelocity.IsNearlyZero()) return;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	FVector ViewLocation;
	FRotator ViewRotation;
	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

	// 회전 축
	FVector YawAxis = FVector::UpVector;
	FVector PitchAxis = ViewRotation.RotateVector(FVector::RightVector);

	// 회전 계산
	FQuat YawQuat(YawAxis, FMath::DegreesToRadians(-RotateVelocity.X * YawRotateSpeed * DeltaTime));
	FQuat PitchQuat(PitchAxis, FMath::DegreesToRadians(-RotateVelocity.Y * PitchRotateSpeed * DeltaTime));

	FQuat NewQuat = PitchQuat * YawQuat * GetActorQuat();
	SetActorRotation(NewQuat);

	// 감속
	RotateVelocity = FMath::Vector2DInterpTo(RotateVelocity, FVector2D::ZeroVector, DeltaTime, RotationDamping);
}


void ASRSmoothRotateActor::AddRotationInput(FVector2D DeltaInput)
{
	RotateVelocity += DeltaInput * 5.0f;
}
