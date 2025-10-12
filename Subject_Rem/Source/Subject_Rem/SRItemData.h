// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "GameplayTagContainer.h"
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
	FText Name;

	/*아이템 설명*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true))
	FText PickupDescription;

	/*인벤토리, 단서조합 ... UI에 들어갈 아이콘*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon;

	/*아이템 메시 데이터*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> Mesh;

	// 자막 사용 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Caption")
	bool bShowPickupCaption = false;

	// 자막 DT(있을 때만 노출)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Caption",
			  meta = (EditCondition = "bShowPickupCaption"))
	FDataTableRowHandle PickupCaptionRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRequiredDevice= false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bRequiredDevice"))
	FName RequiredDeviceId;
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
	TArray<FName> ClueIds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle ClueCombineResult; // → FSRClueMapData
};

USTRUCT(BlueprintType)
struct FSRClueMapData : public FTableRowBase
{
	GENERATED_BODY()

public:
	/*아이템 구분 Id */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Id;

	/*아이템 이름*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	/*아이템 설명*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true))
	FText Description;

	/*단서 조합 후 즉시 증가하는 스트레스 증가량 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ImmediateStessIncrease;
	
	/*단서 조합 후 주기마다 증가하는 스트레스 증가량 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPeriodicStressIncrease PeriodicStressIncrease;

	/*진실, 거짓단서 유무*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bResult;

	// 자막 사용 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Caption")
	bool bShowCaption = false;

	// 자막 DT(있을 때만 노출)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Caption", meta = (EditCondition = "bShowPickupCaption"))
	FDataTableRowHandle CaptionRow;
};

UENUM(BlueprintType)
enum class EQuickSlotType : uint8
{
	Default,   // 일반 퀵슬롯 (0,1,2)
	SpecialR   // R 키 전용
};

USTRUCT(BlueprintType)
struct FSRConsumeData : public FTableRowBase
{
	GENERATED_BODY()

public:
	//사용시 즉시 감소하는 스트레스 감소량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ImmediateStessDecrease;

	//주기적으로 증가하는 스트레스 증가 캔슬
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPeriodicStressIncreaseCancle = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AutoRegistQuickSlot = false;

	// 어떤 퀵슬롯에 들어갈지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuickSlotType SlotType = EQuickSlotType::Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag UseEventTag;
};

USTRUCT(BlueprintType)
struct FSRDeviceItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	//3 or 1 - 오디오조합기계 or 슈퍼컴퓨터
	//오디오조합기계 -> ClueMap 생성, 슈퍼컴퓨터 -> 새로운 Clue획득
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 UsingClueNum;

	// 이 디바이스에서 허용되는 아이템(단서) Id 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Device")
	TArray<FName> AllowedItemIds;
};

USTRUCT(BlueprintType)
struct FSRCaptionData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;

	//이어서 재생할 자막 RowName ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NextTextRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Skipable;
};

USTRUCT(BlueprintType)
struct FSRClueMapUIData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) FSRClueMapData ClueMap;
	UPROPERTY(BlueprintReadOnly) TArray<FName> ClueIds;
	UPROPERTY(BlueprintReadOnly) TArray<FText> ClueNames;

	UPROPERTY(BlueprintReadOnly) TArray<TSoftObjectPtr<UTexture2D>> ClueIcons;
	/*진실, 거짓단서 유무*/
	UPROPERTY(BlueprintReadWrite)
	bool bResult;
};

USTRUCT(BlueprintType)
struct FSRDeviceUIData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSRItemBaseData Base;   // Id, Icon, Name 등

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UsingSlotNum = 2;      // UsingClueNum (1/2/3)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> AllowedItemIds;
};
