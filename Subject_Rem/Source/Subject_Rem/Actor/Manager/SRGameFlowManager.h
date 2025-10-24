// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "SRGameFlowManager.generated.h"


/**
 * 튜토리얼 단계 정보를 담는 구조체
 */
class UInputMappingContext;

//USTRUCT()
//struct FFlowRuntimeState
//{
//    GENERATED_BODY()
//
//    UPROPERTY() FGameFlowInfo Info;
//    int32 Progress = 0;        // 인스턴스별 진행 카운트 (필수)
//    
//};
UENUM(BlueprintType)
enum class ESequenceCueTiming : uint8
{
    OnStart,
    OnComplete
};


USTRUCT(BlueprintType)
struct FGameFlowInfo
{
    GENERATED_BODY()

    // 튜토리얼 단계 ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flow")
    FGameplayTag ID;

    // 이 단계에서 달성해야 하는 목표 태그
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flow")
    FGameplayTag ObjectivesTag;

    // 다음 튜토리얼 단계 ID (없으면 튜토리얼 종료)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flow")
    FGameplayTag NextFlow;

    //단계에서 허용할 입력 매핑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UInputMappingContext*> AllowedInputContexts;

    //같은 태그가 몇번 필요한지 예) 초반 아이템 줍기 2번 이후 Clue 조합 튜토리얼로 넘어가도록 하기 위해
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flow")
    int32 RequiredCount = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bShownCaption;

    UPROPERTY(EditAnywhere, meta = (EditCondition = "bShownCaption"))
    FName CaptionRow;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flow", meta = (EditCondition = "bShownCaption"))
    ESequenceCueTiming CaptionTiming = ESequenceCueTiming::OnComplete; // 기본값: 기존 동

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flow|LevelSequence")
    bool bPlayLevelSequence = false;

    // 어떤 타이밍에 시퀀스 재생할지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flow|LevelSequence", meta = (EditCondition = "bPlayLevelSequence"))
    ESequenceCueTiming SequenceTiming = ESequenceCueTiming::OnComplete;

    // 재생할 시퀀스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flow|LevelSequence", meta = (EditCondition = "bPlayLevelSequence"))
    TSoftObjectPtr<class ULevelSequence> LevelSequence;
};

USTRUCT(BlueprintType)
struct FEndingFlow
{
    GENERATED_BODY()


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flow")
    FName Id;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flow")
    bool bShownCaption;

    UPROPERTY(EditAnywhere, meta = (EditCondition = "bShownCaption"))
    FDataTableRowHandle CaptionRow;
};

/**
 * 게임플로우매니저 - 기존 튜토리얼 매니저를 재구성함, 튜토리얼이 아니라 게임 
 * 목표 달성 시 다음 단계로 자동 전환하는 클래스
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFlowStartedSignature, FGameplayTag, ObjectiveTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFlowCompletedSignature, FGameplayTag, CompleteId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestPlayCaptionRow, const FName&, RowName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCaptionEndedSignature, const FName&, RowName);

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTutorialStepChanged, FGameplayTag, NewTutorialID);
//TODO: TutorialManager가 아니라, GameFlowManager로 수정하고, 기능을 좀 많이 바꿔야할듯.플레이어의 행동 + 자막의 완료 시각이 플로우랑 많이 연관됨
//이벤트 허브 느낌으로 접근해야할거같다.
UCLASS()
class SUBJECT_REM_API ASRGameFlowManager : public AActor
{
    GENERATED_BODY()

public:
    ASRGameFlowManager();

    UFUNCTION(BlueprintCallable)
    void EnqueueFlows(const TArray<FGameFlowInfo>& NewFlows, bool bStartIfIdle = true);

    // 병렬 플로우 추가(주입)용
    UFUNCTION(BlueprintCallable, Category = "Flow|Parallel")
    void AddParallelFlows(const TArray<FGameFlowInfo>& NewFlows);

    UFUNCTION(BlueprintCallable, Category = "Flow|Bindings")
    void RegisterActorForObjectiveTag(const TMap<FGameplayTag, AActor*>& Map);

    UFUNCTION(BlueprintCallable, Category = "Flow|Bindings")
    void UnregisterActorForObjectiveTag(FGameplayTag Tag, AActor* Actor);

    /** 외부에서 플로우 목표를 달성했음을 알릴 때 호출 */
    UFUNCTION(BlueprintCallable)
    void NotifyObjectiveCompleted(FGameplayTag ObjectiveTag);

    UFUNCTION()
    void NotifyFlowCompleteId(FGameplayTag FlowIdTag);

    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    FGameplayTag GetCurrentFlowID() const { return CurrentFlowID; }

    /** 현재 목표 태그 반환 */
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    FGameplayTag GetCurrentObjectiveTag() const { return CurrentObjectiveTag; }

    UFUNCTION(BlueprintCallable, Category = "EndingFlow")
    void ExecuteEndingFlow();

    UFUNCTION(BlueprintImplementableEvent)
    void PlayLevelSequnce(ULevelSequence* PlayedSequnce);

    //UPROPERTY(BlueprintAssignable, Category = "Tutorial")
    //FTutorialStepChanged  OnTutorialStepChanged;
    void StartFlow();

    UPROPERTY(BlueprintAssignable)
    FFlowStartedSignature OnFlowStartDelegate;

    UPROPERTY(BlueprintAssignable)
    FFlowCompletedSignature OnFlowCompleteDelegate;

    UPROPERTY(BlueprintAssignable)
    FOnRequestPlayCaptionRow OnRequestPlayCaptionRow; 

    UPROPERTY(BlueprintAssignable)
    FOnCaptionEndedSignature OnCaptionTypewriterEnd; //자막 재생 끝남 델리게이트

    UPROPERTY(EditAnywhere, Category = "Flow")
    TArray<FGameFlowInfo> SequenceFlowInfos;

    UPROPERTY(EditAnywhere, Category = "Flow")
    TArray<FGameFlowInfo> ParallelFlows;

    UPROPERTY(EditAnywhere)
    TMap<FName,AActor*> EnabledActorByCaptionRowEnded;

    UPROPERTY(EditAnywhere)
    TMap<FName, AActor*> EnabledActorByCaptionRowStart;

    UPROPERTY(EditAnywhere)
    TMap<FGameplayTag, AActor*> EnabledActorByObjectiveTag;

    UPROPERTY(EditAnywhere, Category = "Flow|Bindings")
    TMap<FName, FGameplayTag> CaptionEndToObjectiveTag;

    UPROPERTY(EditAnywhere, Category = "Ending")
    TArray<FGameFlowInfo> TrueEndingFlow;

    UPROPERTY(EditAnywhere, Category = "Ending")
    TArray<FGameFlowInfo> FalseEndingFlow;
    
    UPROPERTY(EditAnywhere, Category = "Ending")
    FGameplayTag EndingTriggerFlowId; // "시퀀스 마지막 단계"의 ID

    UPROPERTY(BlueprintAssignable)
    FFlowCompletedSignature OnEndingFlowCompleteDelegate;
private:
    UFUNCTION()
    void RequestShowingCaption(const FName& CaptionRow);

    UFUNCTION()
    void OnCaptionEnded(const FName& RowName);

    UFUNCTION()
    void OnActorEnableByCaptionStart(const FName& RowName);

    void DoNextFlow(FGameFlowInfo* Current, FGameplayTag CompletedTag);

    void ActivateActorsForObjectiveTag(const FGameplayTag& CompletedTag);

    void ResolveAndEnableActor(AActor* ActorPtr);

    void HandleParallelObjectiveCompleted(const FGameplayTag& CompletedTag);

protected:
    virtual void BeginPlay() override;

      void SetupFlow(FGameplayTag TutorialID);

    void SetupFlowByIndex(int32 Index);
    void CompleteAndPopCurrentFlow(FGameplayTag CompletedTag);

    void ClearAllFlowsAndProgress();
    /** 튜토리얼 정보 검색 */
    FGameFlowInfo* FindNextFlowInfo(FGameplayTag TutorialID);

    /** 현재 튜토리얼 단계 ID */
    UPROPERTY(VisibleAnywhere, Category = "Tutorial|State")
    FGameplayTag CurrentFlowID;

    /** 현재 목표 태그 */
    UPROPERTY(VisibleAnywhere, Category = "Tutorial|State")
    FGameplayTag CurrentObjectiveTag;

    // 현재 각 목표 태그가 몇 번 완료되었는지를 추적
    UPROPERTY()
    TMap<FGameplayTag, int32> ObjectiveProgress;

    // 병렬 진행도
    UPROPERTY()
    TMap<FGameplayTag, int32> ParallelProgress;   

 

};
