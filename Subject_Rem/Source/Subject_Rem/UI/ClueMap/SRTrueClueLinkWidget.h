// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRTrueClueLinkWidget.generated.h"

/**
 * 
 */
UCLASS()
class SUBJECT_REM_API USRTrueClueLinkWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	FORCEINLINE FName GetClueAName() const { return ClueAName; };
	FORCEINLINE FName GetClueBName() const { return ClueBName; };
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName ClueAName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName ClueBName;
};
