// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "SRClueData.generated.h"
/**
 * 
 */
USTRUCT()
struct FSRClueData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FSRClueData() : Name(TEXT("")), Description(TEXT("")), Icon(nullptr)
	{

	}

	/*조합에 사용될 단서 Id */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Id;

	/*단서 이름*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	/*단서 설명*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName Description;

	/*인벤토리, 단서조합 ... UI에 들어갈 아이콘*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;
};
