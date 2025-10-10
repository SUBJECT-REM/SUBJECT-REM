// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRItemData.h"
#include "SRCaptionManagerActor.generated.h"

USTRUCT(BlueprintType)
struct FCaptionPayload
{
	GENERATED_BODY()

	// 선택적 스피커 정보 (없으면 빈 값)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SpeakerName;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCaptionRequestedSignature, const FName&, RowName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTypewriterCompletedSignature, const FName&, RowName);
UCLASS()
class SUBJECT_REM_API ASRCaptionManagerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASRCaptionManagerActor();

	void NotifyPickupResultToggle(bool bOpen);
	void NotifyInvestigationToggle(bool bOpen);

	UFUNCTION(BlueprintCallable)
	void PlayCaptionImmediateNext(const FName& RowName, bool bIgnorePauses = false);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnCaptionFinished(FName RowName);

	UFUNCTION(BlueprintCallable)
	void EnqueueCaption(const FName& RowName);

	UPROPERTY(BlueprintAssignable)
	FCaptionRequestedSignature CaptionRequestedDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnTypewriterCompletedSignature CaptionTypewriterCompletedDelgate;
	UPROPERTY(BlueprintReadOnly)
	FName Current;

	UPROPERTY(Transient, VisibleAnywhere)
	TArray<FName> PendingQueue;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void TryRunNext();
	//void ApplyStree();
	void PlayCaption();

	UFUNCTION()
	void OnRequestPlayCaptionRow(const FName& RowName);



	bool bPickupResultOpen = false;
	bool bInvestigationOpen = false;
	

};
