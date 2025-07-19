// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "SRItemData.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FPeriodicStressIncrease
{
    GENERATED_BODY()

    // 주기(초 단위)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stress")
    float Interval = 1.0f;

    // 주기마다 증가할 스트레스량
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stress")
    float Amount = 0.0f;
};

USTRUCT(BlueprintType)
struct FSRItemBaseData
{
	GENERATED_BODY()

	/*아이템 구분 Id */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Id;

	/*아이템 이름*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	/*아이템 설명*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Description;

	/*인벤토리, 단서조합 ... UI에 들어갈 아이콘*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	/*아이템 메시 데이터*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;
};

USTRUCT(BlueprintType)
struct FSRItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSRItemBaseData BaseInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle ItemDataTable;
};

USTRUCT(BlueprintType)
struct FSRClueData : public FTableRowBase
{
	GENERATED_BODY()

public:

	/*조합에 사용될 다른 Clue Id */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName OtherCombineClueId;

	/*단서 조합 후 결과 Clue Id*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ClueCombineResultId;
};

USTRUCT(BlueprintType)
struct FSRCombinedClueData : public FTableRowBase
{
	GENERATED_BODY()

public:

	/*조합 결과 나오는 텍스트 문구*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ResultText;

	/*단서 조합 후 즉시 증가하는 스트레스 증가량 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ImmediateStessIncrease;
	
	/*단서 조합 후 주기마다 증가하는 스트레스 증가량 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPeriodicStressIncrease PeriodicStressIncrease;
};
