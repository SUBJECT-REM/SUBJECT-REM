// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "SRStressLocalPlayerSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStopStressChangeDelegate)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStressChangeDelegate, const float)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStressChangeByTimeDelegate, const float, const float)

UCLASS()
class SUBJECT_REM_API USRStressLocalPlayerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
public:
	/* 스트레스 값 증가 및 감소
	* 파라미터에 증가시 +, 감소 시 - 값을 넣으면 됩니다.
	*/
	UFUNCTION(BlueprintCallable, Category = "Stress")
	void ChangeStressAmount(float ChangeValue);

	FTimerHandle ChangeStressTimerHandler;
	/* 스트레스 일정 시간마다 값 증가 및 감소
	* @param 스트레스 증가 및 감소 (증가시 +, 감소시 -)
	* @param 몇초에 한번 감소 및 증가를 하는가 (1.5 -> 1.5초마다 실행.
	*/
	UFUNCTION(BlueprintCallable, Category = "Stress")
	void ChangeStressByTime(float ChangeValue, float Time);
	/*
	*/
	UFUNCTION(BlueprintCallable, Category = "Stress")
	void ClearStressTimer();
	/*
	* @return 현재까지 스트레스 퍼센트 (0-1)
	*/
	float GetStressPercent();

	UFUNCTION(BlueprintCallable)
	void PauseStressTimer();

	UFUNCTION(BlueprintCallable)
	void ResumeStressTimer();

	/* 현재 스트레스 값 반환 */
	UFUNCTION(BlueprintCallable, Category = "Stress")
	FORCEINLINE float GetStress() const { return StressAmount; };

	FORCEINLINE float GetMinStress() const { return MinStress; };
	FORCEINLINE float GetMaxStress() const { return MaxStress; };

	FOnStressChangeDelegate OnStressChangeDelegate;
	FOnStressChangeByTimeDelegate OnStressChangeByTimeDelegate;
	FOnStopStressChangeDelegate OnStopStressChangeDelegate;

private:
	/* 로컬 플레이어의 현재 스트레스 */
	float StressAmount = 0.0f;

	/* 최소/최대 스트레스 값 */
	const float MinStress = 0.0f;
	const float MaxStress = 100.0f;
};
