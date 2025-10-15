// Fill out your copyright notice in the Description page of Project Settings.


#include "SRFunctionLibrary.h"

void USRFunctionLibrary::NormalizeIds(TArray<FName>& Arr)
{
    Arr.Sort([](const FName& L, const FName& R) { return L.LexicalLess(R); });
}

bool USRFunctionLibrary::IsEqualSorted(TArray<FName> A, TArray<FName> B)
{
    if (A.Num() != B.Num()) return false;
    NormalizeIds(A); NormalizeIds(B);
    for (int32 i = 0; i < A.Num(); ++i) if (A[i] != B[i]) return false;
    return true;
}

bool USRFunctionLibrary::DoesRuleMatchInput(const TArray<FName>& InputRaw,
                                                const TArray<FName>& InputNormalized,
                                                const FSRClueCombineRuleData* Rule)
{
    if (!Rule) return false;
    const TArray<FName>& RuleIds = Rule->ClueIds;
    if (RuleIds.Num() != InputRaw.Num()) return false;

    if (RuleIds.Num() == 3) return (RuleIds == InputRaw);

    TArray<FName> NormalizedRule = RuleIds;
    NormalizeIds(NormalizedRule);
    return (NormalizedRule == InputNormalized);
}

int32 USRFunctionLibrary::MakeSeedFromIds(const TArray<FName>& Ids)
{
    const FString SeedStr = FString::JoinBy(Ids, TEXT("|"), [](const FName& N) { return N.ToString(); });
    return FCrc::StrCrc32(*SeedStr);
}

FString USRFunctionLibrary::MakeRuneGibberish(int32 Len, int32 Seed)
{

    static const TCHAR* GRuneAlphabet = TEXT("ᚠᚢᚦᚨᚱᚲᚷᚹᚺᚾᛁᛃᛇᛉᛋᛏᛒᛖᛗᛚᛜᛟᛞ");
    const int32 N = FCString::Strlen(GRuneAlphabet);

    FRandomStream Rand(Seed);
    FString Out; Out.Reserve(Len);

    for (int32 i = 0; i < Len; ++i)
    {
        Out.AppendChar(GRuneAlphabet[Rand.RandRange(0, N - 1)]);
    }
    return Out;
}

FName USRFunctionLibrary::MakeFalseClueIdFrom(const TArray<FName>& ConsumedIds)
{
    const FString Key = FString::JoinBy(ConsumedIds, TEXT("_"), [](const FName& N) { return N.ToString(); });
    return FName(*FString::Printf(TEXT("False_%u"), FCrc::StrCrc32(*Key)));
}
