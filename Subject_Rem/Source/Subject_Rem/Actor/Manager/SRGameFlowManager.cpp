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
#include "Variant_SideScrolling/SideScrollingCharacter.h"

ASRGameFlowManager::ASRGameFlowManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ASRGameFlowManager::EnqueueFlows(const TArray<FGameFlowInfo>& NewFlows, bool bStartIfIdle)
{
    if (NewFlows.Num() == 0) return;

    // 뒤에 이어붙이기 (원하면 중복 방지 로직도 추가 가능)
    SequenceFlowInfos.Append(NewFlows);

    // 현재 아무 단계도 진행 중이 아니면 즉시 0번부터 시작
    const bool bIdle = !CurrentFlowID.IsValid() && SequenceFlowInfos.Num() > 0;
    if (bStartIfIdle && bIdle)
    {
        SetupFlowByIndex(0);
    }
}

void ASRGameFlowManager::AddParallelFlows(const TArray<FGameFlowInfo>& NewFlows)
{
    if (NewFlows.Num() == 0) return;
    ParallelFlows.Append(NewFlows);
    // 진행도는 요청 오면 그때부터 올라감(ParallelProgress는 필요할 때 자동 생성)
}

void ASRGameFlowManager::RegisterActorForObjectiveTag(const TMap<FGameplayTag, AActor*>& Map)
{
    for (const auto& Pair : Map)
    {
        EnabledActorByObjectiveTag.Add(Pair.Key, Pair.Value);
    }
}

void ASRGameFlowManager::UnregisterActorForObjectiveTag(FGameplayTag Tag, AActor* Actor)
{
    if (!Tag.IsValid()) return;

    AActor** Found = EnabledActorByObjectiveTag.Find(Tag);
    if (Found && *Found == Actor)
    {
        EnabledActorByObjectiveTag.Remove(Tag);
    }
}

void ASRGameFlowManager::OnCaptionEnded(const FName& RowName)
{
    OnCaptionTypewriterEnd.Broadcast(RowName);

    if (AActor* const* FoundPtr = EnabledActorByCaptionRowEnded.Find(RowName))
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

void ASRGameFlowManager::OnActorEnableByCaptionStart(const FName& RowName)
{

    if (AActor* const* FoundPtr = EnabledActorByCaptionRowStart.Find(RowName))
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

    AActor* Target = nullptr;

    // 맵에서 꺼내면서(복사) 동시에 제거
    if (!EnabledActorByObjectiveTag.RemoveAndCopyValue(CompletedTag, Target) || !IsValid(Target))
    {
        return;
    }

    Target->SetActorHiddenInGame(false);

    if (UBoxComponent* Box = Target->FindComponentByClass<UBoxComponent>())
    {
        Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }
    if (UStaticMeshComponent* Mesh = Target->FindComponentByClass<UStaticMeshComponent>())
    {
        Mesh->SetEnableGravity(true);
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

void ASRGameFlowManager::HandleParallelObjectiveCompleted(const FGameplayTag& CompletedTag)
{
    for (int32 i = ParallelFlows.Num() - 1; i >= 0; --i)
    {
        FGameFlowInfo& Flow = ParallelFlows[i];
        if (Flow.ObjectivesTag != CompletedTag)
            continue;

        int32& Count = ParallelProgress.FindOrAdd(Flow.ID);
        ++Count;

        const int32 Need = FMath::Max(1, Flow.RequiredCount);
        if (Count >= Need)
        {
            // 병렬도 완료 시 연출/활성화가 필요하다면 여기서 수행
            if (Flow.bShownCaption && !Flow.CaptionRow.IsNone())
            {
                RequestShowingCaption(Flow.CaptionRow);
            }
            //ActivateActorsForObjectiveTag(CompletedTag);
            ActivateActorsForObjectiveTag(Flow.ID);
            OnFlowCompleteDelegate.Broadcast(Flow.ID);

            ParallelProgress.Remove(Flow.ID);
            ParallelFlows.RemoveAtSwap(i);
        }
    }
}

void ASRGameFlowManager::ExecuteEndingFlow()
{
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!PlayerCharacter)
        return;
    UActorComponent* ActorComp = PlayerCharacter->GetComponentByClass(USRInventoryComponent::StaticClass());
    if (!ActorComp)
        return;
    USRInventoryComponent* InvenComp = Cast<USRInventoryComponent>(ActorComp);
    if (!InvenComp)
        return;

    
    uint8 TrueClueMapNum = InvenComp->GetTrueClueMapData();
    if (TrueClueMapNum >= 9)
    {

    }
    else
    {

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

    if (auto* Caption = Cast<ASRCaptionManagerActor>(
        UGameplayStatics::GetActorOfClass(GetWorld(), ASRCaptionManagerActor::StaticClass())))
    {
        Caption->CaptionTypewriterCompletedDelgate.AddDynamic(this, &ThisClass::OnCaptionEnded);
        Caption->CaptionTypewriterStartDelgate.AddDynamic(this, &ThisClass::OnActorEnableByCaptionStart);
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

    if (SequenceFlowInfos.Num() == 0)
    {
        CurrentFlowID = FGameplayTag();
        CurrentObjectiveTag = FGameplayTag();
        return;
    }

    SetupFlowByIndex(0);

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

    UE_LOG(LogTemp, Warning, TEXT("CurrentFlowId %s"), *CurrentFlowID.GetTagName().ToString());

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
    if (CurrentFlowID == EndingTriggerFlowId /*지정ID*/)
    {
        ExecuteEndingFlow();
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

void ASRGameFlowManager::NotifyObjectiveCompleted(FGameplayTag ObjectiveTag)
{

    HandleParallelObjectiveCompleted(ObjectiveTag);
    ActivateActorsForObjectiveTag(ObjectiveTag);
   
    //기존 시퀀스 플로우
    if (ObjectiveTag != CurrentObjectiveTag) return;

    int32& Count = ObjectiveProgress.FindOrAdd(ObjectiveTag);
    ++Count;

    FGameFlowInfo* Info = (SequenceFlowInfos.Num() > 0 && SequenceFlowInfos[0].ID == CurrentFlowID)
        ? &SequenceFlowInfos[0] : FindNextFlowInfo(CurrentFlowID);
    if (!Info) return;

    UE_LOG(LogTemp, Log, TEXT("Objective %s Progress: %d / %d"),
        *ObjectiveTag.ToString(), Count, Info->RequiredCount);

    if (Count >= Info->RequiredCount)
    {
        if (Info->bShownCaption && !Info->CaptionRow.IsNone())
        {
            RequestShowingCaption(Info->CaptionRow);
        }

        CompleteAndPopCurrentFlow(ObjectiveTag);
        return;
    }

}

void ASRGameFlowManager::NotifyFlowCompleteId(FGameplayTag FlowIdTag)
{

}
