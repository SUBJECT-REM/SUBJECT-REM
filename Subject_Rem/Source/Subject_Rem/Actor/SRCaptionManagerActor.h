// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

UCLASS()
class SUBJECT_REM_API ASRCaptionManagerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASRCaptionManagerActor();

	UFUNCTION(BlueprintCallable)
	void RequestCaptionShowing(const FName& RowName);

	UPROPERTY(BlueprintAssignable)
	FCaptionRequestedSignature CaptionRequestedDelegate;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
