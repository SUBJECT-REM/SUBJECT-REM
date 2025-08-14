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
	//UE_LOG(LogTemp, Log, TEXT("Change Stress By Time"));
	GetWorld()->GetTimerManager().SetTimer(ChangeStressTimerHandler, [this, ChangeValue, Time]()
	{
			ChangeStressAmount(ChangeValue);
			ChangeStressByTime(ChangeValue, Time);
	}, Time, false);

	OnStressChangeByTimeDelegate.Broadcast(ChangeValue, Time);
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
