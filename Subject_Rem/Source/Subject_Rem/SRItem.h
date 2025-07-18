// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SRItemData.h"
#include "SRItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SUBJECT_REM_API USRItem : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetItemData(const FSRItemData& Data);

	UFUNCTION(BlueprintCallable)
	const FSRItemData& GetItemData() const { return ItemData; }
private:
	FSRItemData ItemData;
};
