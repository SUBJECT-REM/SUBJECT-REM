// Copyright Epic Games, Inc. All Rights Reserved.


#include "SideScrollingCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Component/SRStressHandlerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Engine/World.h"
#include "SideScrollingInteractable.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

ASideScrollingCharacter::ASideScrollingCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// create the camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);

	Camera->SetRelativeLocationAndRotation(FVector(0.0f, 300.0f, 0.0f), FRotator(0.0f, -90.0f, 0.0f));

	// configure the collision capsule
	GetCapsuleComponent()->SetCapsuleSize(35.0f, 90.0f);

	// configure the Pawn properties
	bUseControllerRotationYaw = false;

	// configure the character movement component
	GetCharacterMovement()->GravityScale = 1.75f;
	GetCharacterMovement()->MaxAcceleration = 1500.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->Mass = 500.0f;

	GetCharacterMovement()->SetWalkableFloorAngle(75.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	//걷기 상태에서 감속률 - 값이 높을수록 빨리 멈춤
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->bIgnoreBaseRotation = true;

	GetCharacterMovement()->PerchRadiusThreshold = 15.0f;
	GetCharacterMovement()->LedgeCheckThreshold = 6.0f;

	//점프 높이
	GetCharacterMovement()->JumpZVelocity = 750.0f;
	//0.0 (공중에서 조종 불가) ~ 1.0 (지상과 비슷하게 조작가능)
	GetCharacterMovement()->AirControl = 1.0f;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 750.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 1.0f, 0.0f));
	GetCharacterMovement()->bConstrainToPlane = true;


	// enable double jump - 1단 점프하려면 1로 수정
	JumpMaxCount = 2;
}

void ASideScrollingCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASideScrollingCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASideScrollingCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASideScrollingCharacter::DoJumpEnd);

		// Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ASideScrollingCharacter::DoInteract);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASideScrollingCharacter::Move);

		// Dropping from platform
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &ASideScrollingCharacter::Drop);
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Completed, this, &ASideScrollingCharacter::DropReleased);

		//UseItem -> 여기에 키 넣어주면 됌
		EnhancedInputComponent->BindAction(UseItemNum1Actor, ETriggerEvent::Triggered, this, &ASideScrollingCharacter::UseItemNum1);
		EnhancedInputComponent->BindAction(UseItemNum2Acton, ETriggerEvent::Triggered, this, &ASideScrollingCharacter::UseItemNum2);
		EnhancedInputComponent->BindAction(UseItemNum3Acton, ETriggerEvent::Triggered, this, &ASideScrollingCharacter::UseItemNum3);
	}
}

void ASideScrollingCharacter::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// 캐릭터가 떨어지고 있는 중이 아니라면 return
	if (!GetCharacterMovement()->IsFalling())
	{
		return;
	}

	// ensure the colliding component is valid
	if (OtherComp)
	{
		// ensure the component is movable and simulating physics
		if (OtherComp->Mobility == EComponentMobility::Movable && OtherComp->IsSimulatingPhysics())
		{
			const FVector PushDir = FVector(ActionValueY > 0.0f ? 1.0f : -1.0f, 0.0f, 0.0f);

			// push the component away
			OtherComp->AddImpulse(PushDir * JumpPushImpulse, NAME_None, true);
		}
	}
}

void ASideScrollingCharacter::Landed(const FHitResult& Hit)
{
	// reset the double jump
	bHasDoubleJumped = false;
}

void ASideScrollingCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MoveVector.Y);
}

void ASideScrollingCharacter::Drop(const FInputActionValue& Value)
{
	// route the input	
	DoDrop(Value.Get<float>());
}

void ASideScrollingCharacter::DropReleased(const FInputActionValue& Value)
{
	// reset the input
	DoDrop(0.0f);
}

void ASideScrollingCharacter::DoMove(float Forward)
{
	// save the movement values
	ActionValueY = Forward;

	// figure out the movement direction
	const FVector MoveDir = FVector(1.0f, Forward > 0.0f ? 0.1f : -0.1f, 0.0f);

	// apply the movement input
	AddMovementInput(MoveDir, Forward);
	
}

void ASideScrollingCharacter::DoDrop(float Value)
{
	// save the movement value
	DropValue = Value;
}

void ASideScrollingCharacter::DoJumpStart()
{
	// handle advanced jump behaviors
	MultiJump();
}

void ASideScrollingCharacter::DoJumpEnd()
{
	StopJumping();
}

void ASideScrollingCharacter::DoInteract()
{
	// do a sphere trace to look for interactive objects
	FHitResult OutHit;

	//현재 위치에서 팡의 방향으로 SphereTrace를 한다.
	const FVector Start = GetActorLocation();
	const FVector End = Start + FVector(100.0f, 0.0f, 0.0f);

	FCollisionShape ColSphere;
	ColSphere.SetSphere(InteractionRadius);

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->SweepSingleByObjectType(OutHit, Start, End, FQuat::Identity, ObjectParams, ColSphere, QueryParams))
	{
		// have we hit an interactable?
		if (ISideScrollingInteractable* Interactable = Cast<ISideScrollingInteractable>(OutHit.GetActor()))
		{
			// interact
			Interactable->Interaction(this);
		}
	}
}

void ASideScrollingCharacter::MultiJump()
{
	// Platform아래로 떨어지고 싶어 하는지?
	if (DropValue > 0.0f)
	{
		//발 아래 Platform의 ObjectType이 SoftCollision이면 Platform아래로 내려갈 수 있다.
		CheckForSoftCollision();
		return;
	}

	// reset the drop value
	DropValue = 0.0f;

	// 떨어지는 중이 아니라면 Jump를 수행한다.
	if (!GetCharacterMovement()->IsFalling())
	{
		Jump();
		return;
	}

	//일단 기본 템플릿 테스트를 위해서 남김 - 삭제 예정
	if (!bHasDoubleJumped)
	{
		// raise the double jump flag
		bHasDoubleJumped = true;

		// let the CMC handle jump
		Jump();
	}
}

void ASideScrollingCharacter::CheckForSoftCollision()
{
	// reset the drop value
	DropValue = 0.0f;

	// trace down 
	FHitResult OutHit;

	const FVector Start = GetActorLocation();
	const FVector End = Start + (FVector::DownVector * SoftCollisionTraceDistance);

	//아래를 뚫고 내려가고 싶으면 위의 Mesh가 SoftCollisionObjectType이어야한다.
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(SoftCollisionObjectType);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByObjectType(OutHit, Start, End, ObjectParams, QueryParams);

	// Trace 결과가 SofrCollision이 맞는가?
	if (OutHit.GetActor())
	{
		// drop through the floor
		SetSoftCollision(true);
	}
}

void ASideScrollingCharacter::SetSoftCollision(bool bEnabled)
{
	// enable or disable collision response to the soft collision channel
	GetCapsuleComponent()->SetCollisionResponseToChannel(SoftCollisionObjectType, bEnabled ? ECR_Ignore : ECR_Block);
}

bool ASideScrollingCharacter::HasDoubleJumped() const
{
	return bHasDoubleJumped;
}

void ASideScrollingCharacter::UseItemNum1()
{
	UE_LOG(LogTemp, Log, TEXT("UseItemNum1"));
}

void ASideScrollingCharacter::UseItemNum2()
{
	UE_LOG(LogTemp, Log, TEXT("UseItemNum2"));
}

void ASideScrollingCharacter::UseItemNum3()
{
	UE_LOG(LogTemp, Log, TEXT("UseItemNum3"));
}

