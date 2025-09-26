// SRGameFlowManager.cpp

#include "SRGameFlowManager.h"
#include "SRGameplayTags.h"
#include "Actor/Manager/SRCaptionManagerActor.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Subsystem/SRInputLocalPlayerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Component/SRInventoryComponent.h"

ASRGameFlowManager::ASRGameFlowManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ASRGameFlowManager::OnCaptionEnded(const FName& RowName)
{
    //특정 아이템 스폰 또는 활성화 dt를 사용해서 순회하면될거같은데




    //위젯에서 구독해 하이라이트 표시
    OnCaptionTypewriterEnd.Broadcast(RowName);
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

    StartFirstFlow();
}

void ASRGameFlowManager::StartFirstFlow()
{
    if (TutorialInfos.Num() > 0)
    {
        SetupFlow(TutorialInfos[0].ID);
    }
}

FGameFlowInfo* ASRGameFlowManager::FindNextFlowInfo(FGameplayTag TutorialID)
{
    return TutorialInfos.FindByPredicate([&](const FGameFlowInfo& Info) {
        return Info.ID == TutorialID;
    });
}

void ASRGameFlowManager::SetupFlow(FGameplayTag TutorialID)
{
    FGameFlowInfo* Info = FindNextFlowInfo(TutorialID);
    if (!Info) return;

    CurrentFlowID = Info->ID;
    CurrentObjectiveTag = Info->ObjectivesTag;
    OnFlowStartDelegate.Broadcast(TutorialID);

    if (Info->AllowedInputContexts.Num() > 0)
    {
        if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
        {
            if (USRInputLocalPlayerSubsystem* Subsystem =
                    ULocalPlayer::GetSubsystem<USRInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
            {
                // 한 번에 교체할 세트 구성
                TArray<FIMCEntry> StageContexts;
                for (UInputMappingContext* Context : Info->AllowedInputContexts)
                {
                    if (Context)
                    {
                        // 튜토리얼 단계용 우선순위 규칙 (예: 10)
                        StageContexts.Emplace(Context, /*Priority=*/10);
                    }
                }

                // 통째로 적용
                Subsystem->ReplaceContexts(StageContexts);
            }
        }
    }
    // 여기서 UI 표시, 안내 메시지 등 실행 가능
}

void ASRGameFlowManager::NotifyObjectiveCompleted(FGameplayTag CompletedTag)
{
    // 현재 단계 목표와 동일한지 확인
    if (CompletedTag != CurrentObjectiveTag)
        return;

    FGameFlowInfo* Info = FindNextFlowInfo(CurrentFlowID);
    if (!Info) return;

    // 누적 횟수 증가
    int32& Count = ObjectiveProgress.FindOrAdd(CompletedTag);
    Count++;

    UE_LOG(LogTemp, Log, TEXT("Objective %s Progress: %d / %d"), *CompletedTag.ToString(), Count, Info->RequiredCount);

    if (Count >= Info->RequiredCount)
    {
        DoNextFlow(Info, CompletedTag);

        if (Info->bShownCaption && !Info->CaptionRow.IsNone())
        {
            RequestShowingCaption(Info->CaptionRow);
        }
    }

    return;
}
