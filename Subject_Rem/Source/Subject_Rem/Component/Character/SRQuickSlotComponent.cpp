// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Character/SRQuickSlotComponent.h"
#include "Subsystem/SRStressLocalPlayerSubsystem.h"

// Sets default values for this component's properties
USRQuickSlotComponent::USRQuickSlotComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void USRQuickSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USRQuickSlotComponent::PressQuickSlot(uint8 QuickSlotNum)
{
	UseQuickSlotItem(QuickSlotNum);
}


void USRQuickSlotComponent::UseQuickSlotItem(uint8 QuickSlotNum)
{
	UE_LOG(LogTemp, Log, TEXT("UseQuickSlot : %d"), QuickSlotNum);

	//Test - 필요하면 아래 if - else 문 삭제해도 됩니다.
	if (QuickSlotNum == 1)
	{
		if (ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController())
		{
			if (USRStressLocalPlayerSubsystem* StressSubsystem = LocalPlayer->GetSubsystem<USRStressLocalPlayerSubsystem>())
			{
				StressSubsystem->ChangeStressAmount(1.0f);
			}
		}
	}
	else if (QuickSlotNum == 2)
	{
		if (ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController())
		{
			if (USRStressLocalPlayerSubsystem* StressSubsystem = LocalPlayer->GetSubsystem<USRStressLocalPlayerSubsystem>())
			{
				StressSubsystem->ChangeStressAmount(-1.0f);
			}
		}
	}
	else if (QuickSlotNum == 3)
	{
		if (ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController())
		{
			if (USRStressLocalPlayerSubsystem* StressSubsystem = LocalPlayer->GetSubsystem<USRStressLocalPlayerSubsystem>())
			{
				StressSubsystem->ChangeStressByTime(5.0f, 1.0f);
			}
		}
	}
	//Test 끝
}


// Called every frame
void USRQuickSlotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//생성자에서 비활성화했습니다.
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

