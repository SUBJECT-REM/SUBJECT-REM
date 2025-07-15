// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "SRClueCombineData.generated.h"


USTRUCT()
struct FSRClueCombineData : public FTableRowBase
{
	GENERATED_BODY()

	FSRClueCombineData() : ClueId1(TEXT("")), ClueId2(TEXT("")), ReseultClueId(TEXT(""))
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ClueId1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ClueId2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ReseultClueId;
};
