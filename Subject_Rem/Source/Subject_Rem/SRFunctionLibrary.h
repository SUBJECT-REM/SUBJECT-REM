// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SRItemData.h"
#include "SRFunctionLibrary.generated.h"

/**
 * 
 */

UCLASS()
class SUBJECT_REM_API USRFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
    // 정렬/비교
    static void NormalizeIds(TArray<FName>& Arr);

    static bool IsEqualSorted(TArray<FName> A, TArray<FName> B);

    // 룰 매칭
    static bool DoesRuleMatchInput(const TArray<FName>& InputRaw,
                                   const TArray<FName>& InputNormalized,
                                   const FSRClueCombineRuleData* Rule);

    static int32 MakeSeedFromIds(const TArray<FName>& Ids);

    static FString MakeRuneGibberish(int32 Len, int32 Seed);

    static FName MakeFalseClueIdFrom(const TArray<FName>& ConsumedIds);
};
