// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/SRStressLocalPlayerSubsystem.h"

void USRStressLocalPlayerSubsystem::ChangeStressAmount(const float ChangeValue)
{
	StressAmount = FMath::Clamp(StressAmount + ChangeValue, MinStress, MaxStress);

	if (StressAmount == MaxStress)
	{
		ClearStressTimer();
	}
	UE_LOG(LogTemp, Warning, TEXT("StressAmount  :%f"), StressAmount);
	OnStressChangeDelegate.Broadcast(StressAmount);
	OnStopStressChangeDelegate.Broadcast();
}

void USRStressLocalPlayerSubsystem::ChangeStressByTime(const float ChangeValue, const float Time)
{
	//둘중 하나라도 0이 들어오면 ClearStressTimer;
	if (ChangeValue == 0.f || Time == 0.f)
	{
		ClearStressTimer();
	}
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
	UE_LOG(LogTemp, Warning, TEXT("Clear Stress Timer "));
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

void USRStressLocalPlayerSubsystem::PauseStressTimer()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(ChangeStressTimerHandler))
	{
		GetWorld()->GetTimerManager().PauseTimer(ChangeStressTimerHandler);
		UE_LOG(LogTemp, Warning, TEXT("Stress Timer Paused"));
	}
}

void USRStressLocalPlayerSubsystem::ResumeStressTimer()
{
	if (GetWorld()->GetTimerManager().IsTimerPaused(ChangeStressTimerHandler))
	{
		GetWorld()->GetTimerManager().UnPauseTimer(ChangeStressTimerHandler);
		UE_LOG(LogTemp, Warning, TEXT("Stress Timer Resumed"));
	}
}
