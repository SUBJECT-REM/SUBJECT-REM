// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRRoomTitleTrrigerActor.generated.h"

// RoomTitle 데이터 구조 (인라인/테이블/데이터애셋 어디서든 재사용)
USTRUCT(BlueprintType)
struct FRoomTitleInfo
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomTitle")
	TSoftObjectPtr<UTexture2D> Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomTitle")
	FVector2D Size = FVector2D(405.84f, 106.32f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomTitle")
	FVector2D Position = FVector2D(757.f, 40.f); // 위젯 내부에서 RenderTranslation 등에 사용

};
class UBoxComponent;

UCLASS()
class SUBJECT_REM_API ASRRoomTitleTrrigerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASRRoomTitleTrrigerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(EditAnywhere)
	FRoomTitleInfo LeftToRightTitleInfo;

	UPROPERTY(EditAnywhere)
	FRoomTitleInfo RightToLeftTitleInfo;

	UPROPERTY(EditAnywhere)
	bool bTriggerOnce = false;

private:
	UFUNCTION()
	void OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
