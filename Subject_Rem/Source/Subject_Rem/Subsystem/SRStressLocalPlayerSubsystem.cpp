// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/SRStressLocalPlayerSubsystem.h"

void USRStressLocalPlayerSubsystem::ChangeStressAmount(float ChangeValue)
{
	StressAmount = FMath::Clamp(StressAmount + ChangeValue, MinStress, MaxStress);

	OnStressChangeDelegate.Broadcast(StressAmount);
}

float USRStressLocalPlayerSubsystem::GetStressPercent()
{
	return (StressAmount - MinStress) / (MaxStress - MinStress);
}
