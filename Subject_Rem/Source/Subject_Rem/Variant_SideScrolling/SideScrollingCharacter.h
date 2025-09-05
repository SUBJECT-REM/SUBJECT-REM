// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SideScrollingCharacter.generated.h"

class UCameraComponent;
class USRQuickSlotComponent;
class USRMouseInputComponent;
class UInputAction;

struct FInputActionValue;

/**
 *  A player-controllable character side scrolling game
 */
UCLASS(abstract)
class ASideScrollingCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	/** Player camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuickSlot", meta = (AllowPrivateAccess = "true"))
	USRQuickSlotComponent* QuickSlotComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MouseInput", meta = (AllowPrivateAccess = "true"))
	USRMouseInputComponent* MouseInputComponent;

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

	/*MouseAction*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* OnMousePressAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* OnMouseTurnAction;

	/*UseItem*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* UseItemNum1Actor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* UseItemNum2Acton;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* UseItemNum3Acton;

	/** Last captured horizontal movement input value */
	float ActionValueY = 0.0f;

	/** Last captured platform drop axis value */
	float DropValue = 0.0f;

	/** If true, this character is moving along the side scrolling axis */
	bool bMovingHorizontally = false;

public:
	
	/** Constructor */
	ASideScrollingCharacter();

protected:
	virtual void BeginPlay() override;

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Forward);

public:


public:
	//MousePress Section
public:
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void OnClickMouse(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void OnMouseTurnAxis(const FInputActionValue& Value);

//UseItemSection
public:

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void QuickSlotNum1();
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void QuickSlotNum2();
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void QuickSlotNum3();

private:
	UPROPERTY(EditDefaultsOnly)
	bool MovingY = false;
};
