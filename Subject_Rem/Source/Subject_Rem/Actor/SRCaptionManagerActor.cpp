// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SRCaptionManagerActor.h"
#include "SRCaptionManagerActor.h"
#include "Subsystem/SRStressLocalPlayerSubsystem.h"
// Sets default values
ASRCaptionManagerActor::ASRCaptionManagerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ASRCaptionManagerActor::NotifyInvestigationToggle(bool bOpen)
{
	bInvestigationOpen = bOpen;

	if (!bInvestigationOpen)
	{
		TryRunNext();
	}
}

void ASRCaptionManagerActor::PlayCaption()
{
	// 자막 Row가 비어 있으면 바로 “끝났다” 처리
	if (Current.CaptionRow.RowName.IsNone())
	{
		OnCaptionFinished(NAME_None);
		return;
	}

	// 위젯/프레젠터가 이 델리게이트를 받아 실제 재생
	CaptionRequestedDelegate.Broadcast(Current.CaptionRow.RowName);
}

void ASRCaptionManagerActor::OnCaptionFinished_Implementation(FName RowName)
{
	//자막 끝났으니 스트레스 적용
	ApplyStree();

	Current = FSRClueMapData{};

}

void ASRCaptionManagerActor::TryRunNext()
{
	if ( bInvestigationOpen || PendingQueue.Num() == 0)
		return;

	Current = PendingQueue[0];
	PendingQueue.RemoveAt(0);

	PlayCaption();
}

void ASRCaptionManagerActor::EnqueueFromClue(const FSRClueMapData& Data)
{
	PendingQueue.Add({ Data });          
	if (!bInvestigationOpen)            
		TryRunNext();
}

void ASRCaptionManagerActor::RequestCaptionShowing(const FName& RowName)
{
	CaptionRequestedDelegate.Broadcast(RowName);
}

void ASRCaptionManagerActor::ApplyStree()
{
	if (ULocalPlayer* LP = GetWorld()->GetFirstLocalPlayerFromController())
	{
        if (auto* SS = LP->GetSubsystem<USRStressLocalPlayerSubsystem>())
        {
			SS->ChangeStressAmount(Current.ImmediateStessIncrease);

			SS->ChangeStressByTime(Current.PeriodicStressIncrease.Amount, Current.PeriodicStressIncrease.Interval);

        }
	}
}

// Called when the game starts or when spawned
void ASRCaptionManagerActor::BeginPlay()
{
	Super::BeginPlay();
	
}



