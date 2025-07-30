// SRTutorialManager.cpp

#include "SRTutorialManager.h"
#include "SRGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
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

    OnTutorialStepChanged.Broadcast(TutorialID);

    if (Info->AllowedInputContexts.Num())
    {
        if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
            {
                Subsystem->ClearAllMappings(); // 이전 입력 제거

                for (UInputMappingContext* Context : Info->AllowedInputContexts)
                {
                    if (Context)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Current Input  Mapping Context :%s"), *Context->GetName());
                        Subsystem->AddMappingContext(Context, 0);
                   
                    }
                }
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

    UE_LOG(LogTemp, Log, TEXT("Objective Completed: %s"), *CompletedTag.ToString());

    FTutorialInfo* Info = FindTutorialInfo(CurrentTutorialID);
    if (!Info) return;

    if (Info->NextTutorial.IsValid())
    {
        SetupTutorial(Info->NextTutorial); // 다음 단계 시작
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Tutorial Finished!"));
    }
}
