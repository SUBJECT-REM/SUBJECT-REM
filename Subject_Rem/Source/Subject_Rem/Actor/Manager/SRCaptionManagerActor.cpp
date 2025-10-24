// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Manager/SRCaptionManagerActor.h"
#include "SRCaptionManagerActor.h"
#include "Subsystem/SRStressLocalPlayerSubsystem.h"
#include "Actor/Manager/SRGameFlowManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASRCaptionManagerActor::ASRCaptionManagerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ASRCaptionManagerActor::NotifyPickupResultToggle(bool bOpen)
{
	bPickupResultOpen = bOpen;
	TryRunNext(); // 닫힐 때 재개
}

void ASRCaptionManagerActor::NotifyInvestigationToggle(bool bOpen)
{
	bInvestigationOpen = bOpen;

	TryRunNext();
}

void ASRCaptionManagerActor::NotifyFadeInOutActive(bool bActive)
{
	 bFadeInOutActive = bActive; 
	 TryRunNext();

}

void ASRCaptionManagerActor::PlayCaptionImmediateNext(const FName& RowName, bool bIgnorePauses)
{
	if (RowName.IsNone()) return;

	// Investigation / Pickup 위젯 열림 중이면 즉시는 막고, 맨 앞에 올려둔 뒤 대기
	if (!bIgnorePauses && (bInvestigationOpen || bPickupResultOpen))
	{
		PendingQueue.Insert(RowName, 0);
		return;
	}

	if (Current.IsNone())
	{
		// 아무것도 재생 중이 아니면 바로 재생
		Current = RowName;
		PlayCaption();
		return;
	}

	// 현재 재생 중이면: 새 Row를 맨 앞에 꽂고, 현 재생을 즉시 종료 → 곧바로 새 Row 재생
	PendingQueue.Insert(RowName, 0);

	// 안전한 “즉시 스킵”: Finish 호출 → Current 비우고 TryRunNext()로 이어짐
	OnCaptionFinished(Current);
}

void ASRCaptionManagerActor::PlayCaption()
{
	// 자막 Row가 비어 있으면 바로 “끝났다” 처리
	if (Current.IsNone())
	{
		OnCaptionFinished(NAME_None);
		return;
	}

	// 위젯/프레젠터가 이 델리게이트를 받아 실제 재생
	CaptionRequestedDelegate.Broadcast(Current);
}

void ASRCaptionManagerActor::OnRequestPlayCaptionRow(const FName& RowName)
{
	EnqueueCaption(RowName);
}

void ASRCaptionManagerActor::OnCaptionFinished_Implementation(FName RowName)
{
	Current = NAME_None;
	CaptionTypewriterCompletedDelgate.Broadcast(RowName);

	TryRunNext();
}

void ASRCaptionManagerActor::TryRunNext()
{
	if (bInvestigationOpen || bPickupResultOpen || bFadeInOutActive ||!Current.IsNone() || PendingQueue.Num() == 0)
		return;

	Current = PendingQueue[0];
	PendingQueue.RemoveAt(0);

	PlayCaption();
}

void ASRCaptionManagerActor::EnqueueCaption(const FName& Data)
{
	if (Data.IsNone()) return;
	PendingQueue.Add(Data);
	TryRunNext();

}

//void ASRCaptionManagerActor::ApplyStree()
//{
//	if (Current.ImmediateStessIncrease == 0.f)
//		return;
//	
//	if (ULocalPlayer* LP = GetWorld()->GetFirstLocalPlayerFromController())
//	{
//        if (auto* SS = LP->GetSubsystem<USRStressLocalPlayerSubsystem>())
//        {
//			SS->ChangeStressAmount(Current.ImmediateStessIncrease);
//			SS->ChangeStressByTime(Current.PeriodicStressIncrease.Amount, Current.PeriodicStressIncrease.Interval);
//
//        }
//	}
//}

// Called when the game starts or when spawned
void ASRCaptionManagerActor::BeginPlay()
{
	Super::BeginPlay();
	
	// GameFlow의 '자막 요청' 이벤트를 구독
	if (auto* Flow = Cast<ASRGameFlowManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ASRGameFlowManager::StaticClass())))
	{
		Flow->OnRequestPlayCaptionRow.AddUniqueDynamic(this, &ThisClass::OnRequestPlayCaptionRow);
	}
}



