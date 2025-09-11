// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "SRTutorialManager.generated.h"


/**
 * 튜토리얼 단계 정보를 담는 구조체
 */
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FTutorialInfo
{
    GENERATED_BODY()

    // 튜토리얼 단계 ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
    FGameplayTag ID;

    // 이 단계에서 달성해야 하는 목표 태그
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
    FGameplayTag ObjectivesTag;

    // 다음 튜토리얼 단계 ID (없으면 튜토리얼 종료)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
    FGameplayTag NextTutorial;

    //단계에서 허용할 입력 매핑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UInputMappingContext*> AllowedInputContexts;

    //같은 태그가 몇번 필요한지 예) 초반 아이템 줍기 2번 이후 Clue 조합 튜토리얼로 넘어가도록 하기 위해
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
    int32 RequiredCount = 1;
};

/**
 * 튜토리얼 매니저: 현재 튜토리얼 상태를 관리하고,
 * 목표 달성 시 다음 단계로 자동 전환하는 클래스
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTutorialStartedSignature, FGameplayTag, ObjectiveTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTutorialCompletedSignature, FGameplayTag, CompleteId);


//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTutorialStepChanged, FGameplayTag, NewTutorialID);
//TODO: TutorialManager가 아니라, GameFlowManager로 수정하고, 기능을 좀 많이 바꿔야할듯.플레이어의 행동 + 자막의 완료 시각이 플로우랑 많이 연관됨
UCLASS()
class SUBJECT_REM_API ASRTutorialManager : public AActor
{
    GENERATED_BODY()

public:
    ASRTutorialManager();

    /** 외부에서 튜토리얼 목표를 달성했음을 알릴 때 호출 */
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void NotifyObjectiveCompleted(FGameplayTag CompletedTag);

    /** 현재 튜토리얼 단계의 ID 반환 */
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    FGameplayTag GetCurrentTutorialID() const { return CurrentTutorialID; }

    /** 현재 목표 태그 반환 */
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    FGameplayTag GetCurrentObjectiveTag() const { return CurrentObjectiveTag; }

    //UPROPERTY(BlueprintAssignable, Category = "Tutorial")
    //FTutorialStepChanged  OnTutorialStepChanged;

    UPROPERTY(BlueprintAssignable)
    FTutorialStartedSignature OnTutorialStartDelegate;

    UPROPERTY(BlueprintAssignable)
    FTutorialCompletedSignature OnTutorialCompleteDelegate;

    /** 모든 튜토리얼 단계 데이터 */
    UPROPERTY(EditAnywhere, Category = "Tutorial|Data")
    TArray<FTutorialInfo> TutorialInfos;

protected:
    virtual void BeginPlay() override;

    /** 첫 번째 튜토리얼 단계 시작 */
    void StartFirstTutorial();

    /** 특정 튜토리얼 단계로 전환 */
    void SetupTutorial(FGameplayTag TutorialID);

    /** 튜토리얼 정보 검색 */
    FTutorialInfo* FindTutorialInfo(FGameplayTag TutorialID);

    /** 현재 튜토리얼 단계 ID */
    UPROPERTY(VisibleAnywhere, Category = "Tutorial|State")
    FGameplayTag CurrentTutorialID;

    /** 현재 목표 태그 */
    UPROPERTY(VisibleAnywhere, Category = "Tutorial|State")
    FGameplayTag CurrentObjectiveTag;

    // 현재 각 목표 태그가 몇 번 완료되었는지를 추적
    UPROPERTY()
    TMap<FGameplayTag, int32> ObjectiveProgress;

 

};
