// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Engine/TimerHandle.h"
#include "SRTutorialComponent.generated.h"

class ASRTutorialManager;
class UWidgetComponent;

/*튜토리얼 매니저를 참조해서 튜토리얼을 목적 태그를 바인딩할 컴포넌트 
  액터의 튜토리얼 관련 위젯 컴포넌트가 있어야만 동작하는것이 아닌 Visible만 관리하는정도면 참조해도 괜찮다고 생각함(..?)
  

*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUBJECT_REM_API USRTutorialComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USRTutorialComponent();

	UFUNCTION(BlueprintCallable)
	void NotifyTutorialObjective(FGameplayTag ObjectiveTag);

	UFUNCTION(BlueprintCallable)
	void SetTutorialWidgetComponent(UWidgetComponent* WidgetComp);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void HandleTutorialStarted(FGameplayTag Tag);

	UFUNCTION()
	void HandleTutorialCompleted(FGameplayTag Tag);

	UFUNCTION()
	void HiddenPlayerTutorialWidgetComponent();

	ASRTutorialManager* TutorialManager;
		
	UWidgetComponent* OwnerTutorialWidgetComp;

	/** 지연 완료할 목표 태그 저장 */
	FGameplayTag PendingObjectiveTag;

	//튜토리얼 완료 지연 - 예) 움직이고 3초뒤 튜토리얼 완료
	UPROPERTY(EditDefaultsOnly)
	float TutorialWidgetHiddenDelay;

	//튜토리얼 아이디
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ExpectedTag;

};
