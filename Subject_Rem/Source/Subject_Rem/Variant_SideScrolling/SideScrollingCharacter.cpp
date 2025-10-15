// Copyright Epic Games, Inc. All Rights Reserved.


#include "SideScrollingCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Component/Character/SRQuickSlotComponent.h"
#include "Component/Character/SRMouseInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Engine/World.h"
#include "SideScrollingInteractable.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Subsystem/SRStressLocalPlayerSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "UI/SRInvestigationMenu.h"
#include "Component/SRInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/Manager/SRGameFlowManager.h"
#include "SRGameplayTags.h"

ASideScrollingCharacter::ASideScrollingCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// create the camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocationAndRotation(FVector(0.0f, 300.0f, 0.0f), FRotator(0.0f, -90.0f, 0.0f));
	QuickSlotComponent = CreateDefaultSubobject<USRQuickSlotComponent>(TEXT("QuickSlotComponent"));
	MouseInputComponent = CreateDefaultSubobject<USRMouseInputComponent>(TEXT("MouseInputComponent"));
	InventoryComponent = CreateDefaultSubobject<USRInventoryComponent>(TEXT("InventoryComponent"));
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

	//GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 1.0f, 0.0f));
	//GetCharacterMovement()->bConstrainToPlane = true;
}

void ASideScrollingCharacter::BeginPlay()
{
	Super::BeginPlay();

	CreateInvestigationMenu();

	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASRGameFlowManager::StaticClass());
	if (FoundActor)
	{
		CachedTutorialMgr = Cast<ASRGameFlowManager>(FoundActor);
	}
}

void ASideScrollingCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{


		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &ASideScrollingCharacter::MoveStart);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASideScrollingCharacter::Move);

		//Mouse Action
		EnhancedInputComponent->BindAction(OnMousePressAction, ETriggerEvent::Triggered, this, &ASideScrollingCharacter::OnClickMouse);
		EnhancedInputComponent->BindAction(OnMouseTurnAction, ETriggerEvent::Triggered, this, &ASideScrollingCharacter::OnMouseTurnAxis);

		//UseItem -> 여기에 키 넣어주면 됌
		EnhancedInputComponent->BindAction(UseItemNum1Actor, ETriggerEvent::Triggered, this, &ASideScrollingCharacter::QuickSlotNum1);
		EnhancedInputComponent->BindAction(UseItemNum2Acton, ETriggerEvent::Triggered, this, &ASideScrollingCharacter::QuickSlotNum2);
		EnhancedInputComponent->BindAction(UseItemNum3Acton, ETriggerEvent::Triggered, this, &ASideScrollingCharacter::QuickSlotNum3);

		//OpenInvenstigation
		EnhancedInputComponent->BindAction(ToggleInvestigationMenuAction, ETriggerEvent::Started, this, &ASideScrollingCharacter::ToggleInvestigationMenu);
	}
}


void ASideScrollingCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MoveVector.Y);

	if (CachedTutorialMgr.IsValid())
	{
		CachedTutorialMgr->NotifyObjectiveCompleted(SRGameplayTags::Tutorial_Objectives_Move);
	}

}

void ASideScrollingCharacter::MoveStart(const FInputActionValue& Value)
{
	if (CachedTutorialMgr.IsValid())
	{
		CachedTutorialMgr->NotifyObjectiveCompleted(SRGameplayTags::Tutorial_Objectives_Move);
	}
}

void ASideScrollingCharacter::DoMove(float Forward)
{
	// save the movement values
	ActionValueY = Forward;

	// figure out the movement direction
	if (MovingY)
	{
		const FVector MoveDir = FVector(0.0f, 1.0f, 0.0f);
		AddMovementInput(MoveDir, Forward);
	}
	else
	{
		const FVector MoveDir = FVector(1.0f, Forward > 0.0f ? 0.1f : -0.1f, 0.0f);
		AddMovementInput(MoveDir, Forward);
	}
}

void ASideScrollingCharacter::OnClickMouse(const FInputActionValue& Value)
{
	bool bIsClick = Value.Get<bool>();
	MouseInputComponent->OnClickMouse(bIsClick);
}

void ASideScrollingCharacter::OnMouseTurnAxis(const FInputActionValue& Value)
{
	FVector2D turnAxis = Value.Get<FVector2D>();
	MouseInputComponent->OnMouseTurnAxis(turnAxis);
}

void ASideScrollingCharacter::QuickSlotNum1()
{
	//입력부와 실행부를 나눕니다.
	//이후 퀵 슬롯과 결합을 하지 않기 위해 Component로 나눕니다.
	check(QuickSlotComponent);
	QuickSlotComponent->PressQuickSlot(1);
}

void ASideScrollingCharacter::QuickSlotNum2()
{
	check(QuickSlotComponent);
	QuickSlotComponent->PressQuickSlot(2);
}

void ASideScrollingCharacter::QuickSlotNum3()
{
	check(QuickSlotComponent);
	QuickSlotComponent->PressQuickSlot(3);
}

void ASideScrollingCharacter::CreateInvestigationMenu()
{
	
	if (InvestigationWidgetClass && InventoryComponent)
	{
		InvestigationWidget = CreateWidget<USRInvestigationMenu>(GetWorld(), InvestigationWidgetClass);
		InvestigationWidget->AddToViewport();
		InvestigationWidget->InitInvestigationMenuWidget(InventoryComponent);
	}
}

void ASideScrollingCharacter::ToggleInvestigationMenu()
{
	if (!InvestigationWidget)
	{
		CreateInvestigationMenu();
	}

	if (InvestigationWidget)
	{
		if (InvestigationWidget->GetVisibility() != ESlateVisibility::Visible)
		{
			InvestigationWidget->ShowWidget();

		}
	}
	
}
