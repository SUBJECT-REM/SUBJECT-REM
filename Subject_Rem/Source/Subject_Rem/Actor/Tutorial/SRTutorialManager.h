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
};

/**
 * 튜토리얼 매니저: 현재 튜토리얼 상태를 관리하고,
 * 목표 달성 시 다음 단계로 자동 전환하는 클래스
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTutorialStepChanged, FGameplayTag, NewTutorialID);

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

    UPROPERTY(BlueprintAssignable, Category = "Tutorial")
    FTutorialStepChanged  OnTutorialStepChanged;
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

    /** 모든 튜토리얼 단계 데이터 */
    UPROPERTY(EditDefaultsOnly, Category = "Tutorial|Data")
    TArray<FTutorialInfo> TutorialInfos;

};
