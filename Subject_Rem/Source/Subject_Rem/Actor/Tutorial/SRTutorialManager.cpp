// SRTutorialManager.cpp

#include "SRTutorialManager.h"
#include "SRGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Subsystem/SRInputLocalPlayerSubsystem.h"
#include "Kismet/GameplayStatics.h"

ASRTutorialManager::ASRTutorialManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ASRTutorialManager::BeginPlay()
{
    Super::BeginPlay();
    StartFirstTutorial();
}

void ASRTutorialManager::StartFirstTutorial()
{
    if (TutorialInfos.Num() > 0)
    {
        SetupTutorial(TutorialInfos[0].ID);
    }
}

FTutorialInfo* ASRTutorialManager::FindTutorialInfo(FGameplayTag TutorialID)
{
    return TutorialInfos.FindByPredicate([&](const FTutorialInfo& Info) {
        return Info.ID == TutorialID;
    });
}

void ASRTutorialManager::SetupTutorial(FGameplayTag TutorialID)
{
    FTutorialInfo* Info = FindTutorialInfo(TutorialID);
    if (!Info) return;

    CurrentTutorialID = Info->ID;
    CurrentObjectiveTag = Info->ObjectivesTag;
    UE_LOG(LogTemp, Warning, TEXT("TutorialStart : ASRTutorialManager, Tag ID : %s"), *TutorialID.ToString());
    OnTutorialStartDelegate.Broadcast(TutorialID);

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
    UE_LOG(LogTemp, Log, TEXT("Tutorial Started: %s"), *CurrentTutorialID.ToString());
}

void ASRTutorialManager::NotifyObjectiveCompleted(FGameplayTag CompletedTag)
{
    // 현재 단계 목표와 동일한지 확인
    if (CompletedTag != CurrentObjectiveTag)
        return;

    FTutorialInfo* Info = FindTutorialInfo(CurrentTutorialID);
    if (!Info) return;

    // 누적 횟수 증가
    int32& Count = ObjectiveProgress.FindOrAdd(CompletedTag);
    Count++;

    UE_LOG(LogTemp, Log, TEXT("Objective %s Progress: %d / %d"), *CompletedTag.ToString(), Count, Info->RequiredCount);

    if (Count >= Info->RequiredCount)
    {
        UE_LOG(LogTemp, Log, TEXT("Objective Fully Completed: %s"), *CompletedTag.ToString());
        
        OnTutorialCompleteDelegate.Broadcast(CurrentTutorialID);
        // 다음 튜토리얼로 진행
        if (Info->NextTutorial.IsValid())
        {
            SetupTutorial(Info->NextTutorial);

            // 진행 상태 초기화
            ObjectiveProgress.Remove(CompletedTag);
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("Tutorial Finished!"));
        }
    }
}
