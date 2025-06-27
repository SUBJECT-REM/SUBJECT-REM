// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SideScrollingCharacter.generated.h"

class UCameraComponent;
class UInputAction;
struct FInputActionValue;

/**
 *  A player-controllable character side scrolling game
 */
UCLASS(abstract)
class ASideScrollingCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Player camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

protected:

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* MoveAction;
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* JumpAction;
	/** Drop from Platform Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* DropAction;
	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* InteractAction;

	/** Max distance that interactive objects can be triggered */
	UPROPERTY(EditAnywhere, Category="Side Scrolling")
	float InteractionRadius = 200.0f;


	/** Impulse to manually push physics objects while we're in midair */
	UPROPERTY(EditAnywhere, Category="Side Scrolling")
	float JumpPushImpulse = 600.0f;

	/** Collision object type to use for soft collision traces (dropping down floors) */
	UPROPERTY(EditAnywhere, Category="Side Scrolling")
	TEnumAsByte<ECollisionChannel> SoftCollisionObjectType;

	/** Distance to trace down during soft collision checks */
	UPROPERTY(EditAnywhere, Category="Side Scrolling")
	float SoftCollisionTraceDistance = 1000.0f;

	/** Last captured horizontal movement input value */
	float ActionValueY = 0.0f;

	/** Last captured platform drop axis value */
	float DropValue = 0.0f;


	/** If true, this character has already double jumped */
	bool bHasDoubleJumped = false;

	/** If true, this character is moving along the side scrolling axis */
	bool bMovingHorizontally = false;

public:
	
	/** Constructor */
	ASideScrollingCharacter();

protected:
	virtual void BeginPlay() override;

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Collision handling */
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	/** Landing handling */
	virtual void Landed(const FHitResult& Hit) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for drop from platform input */
	void Drop(const FInputActionValue& Value);

	/** Called for drop from platform input release */
	void DropReleased(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Forward);

	/** Handles drop inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoDrop(float Value);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	/** Handles interact inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoInteract();

protected:
	//점프 높이 값 설정
	UPROPERTY(EditDefaultsOnly, Category = "Jump")
	float JumpVelocity;
	/** Handles advanced jump logic */
	void MultiJump();
	/** Checks for soft collision with platforms */
	void CheckForSoftCollision();

public:

	/** Sets the soft collision response. True passes, False blocks */
	void SetSoftCollision(bool bEnabled);

public:

	/** Returns true if the character has just double jumped */
	UFUNCTION(BlueprintPure, Category="Side Scrolling")
	bool HasDoubleJumped() const;

};
