// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/SRStressLocalPlayerSubsystem.h"

void USRStressLocalPlayerSubsystem::ChangeStressAmount(const float ChangeValue)
{
	StressAmount = FMath::Clamp(StressAmount + ChangeValue, MinStress, MaxStress);

	if (StressAmount == MaxStress)
	{
		ClearStressTimer();
	}

	OnStressChangeDelegate.Broadcast(StressAmount);
}

void USRStressLocalPlayerSubsystem::ChangeStressByTime(const float ChangeValue, const float Time)
{
	GetWorld()->GetTimerManager().SetTimer(ChangeStressTimerHandler, [this, ChangeValue]()
	{
			ChangeStressAmount(ChangeValue);
	}, Time, true);
}

void USRStressLocalPlayerSubsystem::ClearStressTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(ChangeStressTimerHandler);
}

float USRStressLocalPlayerSubsystem::GetStressPercent()
{
	float StressPercent = (StressAmount - MinStress) / (MaxStress - MinStress);
	if (StressPercent >= 1.0)
	{
		ClearStressTimer();
	}
	return StressPercent;
}
