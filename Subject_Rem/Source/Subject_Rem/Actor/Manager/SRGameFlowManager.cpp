// SRGameFlowManager.cpp

#include "SRGameFlowManager.h"
#include "SRGameplayTags.h"
#include "Actor/Manager/SRCaptionManagerActor.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Subsystem/SRInputLocalPlayerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Component/SRInventoryComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

ASRGameFlowManager::ASRGameFlowManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ASRGameFlowManager::OnCaptionEnded(const FName& RowName)
{
    OnCaptionTypewriterEnd.Broadcast(RowName);

    if (AActor* const* FoundPtr = EnabledActorByCaptionRow.Find(RowName)) 
    {
        if (AActor* Target = *FoundPtr) 
        {
            if (IsValid(Target))
            {
                Target->SetActorHiddenInGame(false);
                UBoxComponent* BoxComp = Target->GetComponentByClass<UBoxComponent>();
                BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            }
        }
    }
}

void ASRGameFlowManager::DoNextFlow(FGameFlowInfo* Current, FGameplayTag CompletedTag)
{

    OnFlowCompleteDelegate.Broadcast(CurrentFlowID);
    // 다음 튜토리얼로 진행
    if (Current->NextFlow.IsValid())
    {
        SetupFlow(Current->NextFlow);

        // 진행 상태 초기화
        ObjectiveProgress.Remove(CompletedTag);
    }
    else
    {
        CurrentFlowID = FGameplayTag();       // 무효화
        CurrentObjectiveTag = FGameplayTag(); // 무효화
    }
    return;
}

void ASRGameFlowManager::ActivateActorsForObjectiveTag(const FGameplayTag& CompletedTag)
{
    if (!CompletedTag.IsValid()) return;

    if (AActor* const* FoundPtr = EnabledActorByObjectiveTag.Find(CompletedTag))
    {
        if (AActor* Target = *FoundPtr)
        {
            if (IsValid(Target))
            {
                Target->SetActorHiddenInGame(false);

                UBoxComponent* BoxComp = Target->GetComponentByClass<UBoxComponent>();
                UStaticMeshComponent*  MeshComp = Target->GetComponentByClass<UStaticMeshComponent>();
                MeshComp->SetEnableGravity(true);
                BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            }
        }
    }
}

void ASRGameFlowManager::ResolveAndEnableActor(AActor* ActorPtr)
{
    if (IsValid(ActorPtr))
    {
        ActorPtr->SetActorHiddenInGame(false);
        ActorPtr->SetActorEnableCollision(true);
    }
}

void ASRGameFlowManager::RequestShowingCaption(const FName& CaptionRow)
{
    if (CaptionRow.IsNone()) return;

    OnRequestPlayCaptionRow.Broadcast(CaptionRow);
}

void ASRGameFlowManager::BeginPlay()
{
    Super::BeginPlay();

    // 로컬 플레이어 인벤토리 구독(프로젝트 상황에 맞게 가져오기)
    if (APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0))
    {
        if (auto* Inv = Pawn->FindComponentByClass<USRInventoryComponent>())
        {
            Inv->ClueCombineCaptionDelegate.AddDynamic(this, &ThisClass::RequestShowingCaption);
        }
    }

    // SRGameFlowManager.cpp (BeginPlay)
    if (auto* Caption = Cast<ASRCaptionManagerActor>(
        UGameplayStatics::GetActorOfClass(GetWorld(), ASRCaptionManagerActor::StaticClass())))
    {
        Caption->CaptionTypewriterCompletedDelgate.AddDynamic(this, &ThisClass::OnCaptionEnded);
    }

    StartFlow();
}

void ASRGameFlowManager::StartFlow()
{
  /*  if (SequenceFlowInfos.Num() > 0)
    {
        SetupFlow(SequenceFlowInfos[0].ID);
    }*/
    if (SequenceFlowInfos.Num() > 0)
    {
        SetupFlowByIndex(0);
    }
}

FGameFlowInfo* ASRGameFlowManager::FindNextFlowInfo(FGameplayTag TutorialID)
{
    return SequenceFlowInfos.FindByPredicate([&](const FGameFlowInfo& Info) {
        return Info.ID == TutorialID;
    });
}

void ASRGameFlowManager::SetupFlow(FGameplayTag TutorialID)
{
    //FGameFlowInfo* Info = FindNextFlowInfo(TutorialID);
    //if (!Info) return;

    //CurrentFlowID = Info->ID;
    //CurrentObjectiveTag = Info->ObjectivesTag;
    //OnFlowStartDelegate.Broadcast(TutorialID);

    //if (Info->AllowedInputContexts.Num() > 0)
    //{
    //    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    //    {
    //        if (USRInputLocalPlayerSubsystem* Subsystem =
    //                ULocalPlayer::GetSubsystem<USRInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
    //        {
    //            // 한 번에 교체할 세트 구성
    //            TArray<FIMCEntry> StageContexts;
    //            for (UInputMappingContext* Context : Info->AllowedInputContexts)
    //            {
    //                if (Context)
    //                {
    //                    // 튜토리얼 단계용 우선순위 규칙 (예: 10)
    //                    StageContexts.Emplace(Context, /*Priority=*/10);
    //                }
    //            }

    //            // 통째로 적용
    //            Subsystem->ReplaceContexts(StageContexts);
    //        }
    //    }
    //}
    //// 여기서 UI 표시, 안내 메시지 등 실행 가능


    if (SequenceFlowInfos.Num() == 0)
    {
        CurrentFlowID = FGameplayTag();
        CurrentObjectiveTag = FGameplayTag();
        return;
    }

    SetupFlowByIndex(0); // ★ 언제나 0번만

}

void ASRGameFlowManager::SetupFlowByIndex(int32 Index)
{
    if (!SequenceFlowInfos.IsValidIndex(Index))
    {
        CurrentFlowID = FGameplayTag();
        CurrentObjectiveTag = FGameplayTag();
        return;
    }

    FGameFlowInfo& Info = SequenceFlowInfos[Index];

    CurrentFlowID = Info.ID;
    CurrentObjectiveTag = Info.ObjectivesTag;
    OnFlowStartDelegate.Broadcast(CurrentFlowID);

    if (Info.AllowedInputContexts.Num() > 0)
    {
        if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
        {
            if (USRInputLocalPlayerSubsystem* Subsystem =
                ULocalPlayer::GetSubsystem<USRInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
            {
                TArray<FIMCEntry> StageContexts;
                for (UInputMappingContext* Context : Info.AllowedInputContexts)
                {
                    if (Context) { StageContexts.Emplace(Context, /*Priority=*/10); }
                }
                Subsystem->ReplaceContexts(StageContexts);
            }
        }
    }
}

void ASRGameFlowManager::CompleteAndPopCurrentFlow(FGameplayTag CompletedTag)
{
    OnFlowCompleteDelegate.Broadcast(CurrentFlowID);
    ObjectiveProgress.Remove(CompletedTag);

    // 일반적으로 0번이 현재 스텝이므로 바로 pop
    if (SequenceFlowInfos.Num() > 0 && SequenceFlowInfos[0].ID == CurrentFlowID)
    {
        SequenceFlowInfos.RemoveAt(0);
    }
    else
    {
        // 안전 장치: 혹시 0번이 아닌 경우 ID로 찾아서 제거
        const int32 Idx = SequenceFlowInfos.IndexOfByPredicate(
            [&](const FGameFlowInfo& Info) { return Info.ID == CurrentFlowID; });
        if (Idx != INDEX_NONE)
        {
            SequenceFlowInfos.RemoveAt(Idx);
        }
    }

    // 남아있으면 다시 0번 세팅, 없으면 종료 상태
    if (SequenceFlowInfos.Num() > 0)
    {
        SetupFlowByIndex(0);
    }
    else
    {
        CurrentFlowID = FGameplayTag();
        CurrentObjectiveTag = FGameplayTag();
    }
}

void ASRGameFlowManager::NotifyObjectiveCompleted(FGameplayTag CompletedTag)
{
    //// 현재 단계 목표와 동일한지 확인
    //ActivateActorsForObjectiveTag(CompletedTag);

    //if (CompletedTag != CurrentObjectiveTag)
    //    return;

    //FGameFlowInfo* Info = FindNextFlowInfo(CurrentFlowID);
    //if (!Info) return;

    //// 누적 횟수 증가
    //int32& Count = ObjectiveProgress.FindOrAdd(CompletedTag);
    //Count++;

    //UE_LOG(LogTemp, Log, TEXT("Objective %s Progress: %d / %d"), *CompletedTag.ToString(), Count, Info->RequiredCount);

    //if (Count >= Info->RequiredCount)
    //{
    //    DoNextFlow(Info, CompletedTag);

    //    if (Info->bShownCaption && !Info->CaptionRow.IsNone())
    //    {
    //        RequestShowingCaption(Info->CaptionRow);
    //    }
    //}

    //return;
    ActivateActorsForObjectiveTag(CompletedTag);


    if (CompletedTag != CurrentObjectiveTag) return;

    int32& Count = ObjectiveProgress.FindOrAdd(CompletedTag);
    ++Count;

    FGameFlowInfo* Info = (SequenceFlowInfos.Num() > 0 && SequenceFlowInfos[0].ID == CurrentFlowID)
        ? &SequenceFlowInfos[0] : FindNextFlowInfo(CurrentFlowID);
    if (!Info) return;

    UE_LOG(LogTemp, Log, TEXT("Objective %s Progress: %d / %d"),
        *CompletedTag.ToString(), Count, Info->RequiredCount);

    if (Count >= Info->RequiredCount)
    {
        if (Info->bShownCaption && !Info->CaptionRow.IsNone())
        {
            RequestShowingCaption(Info->CaptionRow);
        }

        CompleteAndPopCurrentFlow(CompletedTag);
        return;
    }
}
