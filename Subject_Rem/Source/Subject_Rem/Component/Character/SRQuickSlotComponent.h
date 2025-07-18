// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SRQuickSlotComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUBJECT_REM_API USRQuickSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USRQuickSlotComponent();
	//퀵 슬롯 입력을 받아옵니다.
	void PressQuickSlot(uint8 QuickSlotNum);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//실제 QuickSlot에 있는 아이템을 들고와서 사용합니다.
	virtual void UseQuickSlotItem(uint8 QuickSlotNum);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
