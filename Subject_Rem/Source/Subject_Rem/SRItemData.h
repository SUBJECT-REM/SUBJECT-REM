// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "SRItemData.generated.h"
/**
*	[FSRItemData] (아이템들의 기본 데이터 정보가 담긴 데이터 테이블 로우)
			|
			├── FDataTableRowHandle ItemDataTable(Consume, 에 대한 세부정보 ,Clue의 경우 FSRItemData에서 필요한 정보가 다 포함되었음. )
			├── FSRItemBaseData
					├── FName Id;
					├── FName Name;
					├── FName Description
					├── UTexture2D* Icon
					└── UStaticMesh* Mesh

*	[FSRClueCombineData] (Clue조합을 정보를 담긴 데이터 테이블 로우)
			|
			├── FName ClueId1
			├── FName ClueId2
			└── FDataTableRowHandle ClueCombineResult(FSRCombinedClueData - 생성된 ClueMap 정보)


*	[FSRClueMapData]
			|
			├── UTexture2D* Icon
			├── FName ResultText
			├── float ImmediateStressIncrease
			└── FPeriodicStressIncrease
					├── float Interval
					└── float Amount
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
struct FSRClueCombineRuleData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ClueId1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ClueId2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle ClueCombineResult; // → FSRClueMapData
};

USTRUCT(BlueprintType)
struct FSRClueMapData : public FTableRowBase
{
	GENERATED_BODY()

public:
	/*ClueMap Id, 이름, 설명, 아이콘 ,메시*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSRItemBaseData BaseInfo;

	/*단서 조합 후 즉시 증가하는 스트레스 증가량 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ImmediateStessIncrease;
	
	/*단서 조합 후 주기마다 증가하는 스트레스 증가량 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPeriodicStressIncrease PeriodicStressIncrease;

	/*진실, 거짓단서 유무*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bResult;
};
