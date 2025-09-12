// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRItem.h"
#include "SRClueMapCombinedResultWidget.generated.h"

class UButton;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FClueMapCombinedResultClickedSignature, const FSRClueMapData&, Data);

UCLASS()
class SUBJECT_REM_API USRClueMapCombinedResultWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void UpdateLeftRightClueImage(TSoftObjectPtr<UTexture2D> Left, TSoftObjectPtr<UTexture2D> Right);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	UDataTable* ClueDataTable;
	//해당 ClueID로 어떤 Clue인지 확인합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterSelect")
	FName CombinedClueID;

	//Descrtion을 위한 Button
	UPROPERTY(meta = (BindWidget))
	UButton* CombinedClueButton;
	//Button Image; 
	UPROPERTY(meta = (BindWidget))
	UImage* LeftClueImage;
	UPROPERTY(meta = (BindWidget))
	UImage* RightClueImage;

	FClueMapCombinedResultClickedSignature ClueMapCombinedResultClickedDelegate;

	//진실 단서인지 여기서 결정해도 됌.이 아니라 결정할 필요가 없다.
	//그냥 결국 Description에 설명이 나오고
	//진실 단서는 연결을 하는 것이니까. - 이 Widget에 연결과 관련해서만 수정하면 된다.
	//그럼 연결은 내 생각에 ClueMapWidget에서 하는게 나을 것 같다.
	//그 이유는 여긴 ClueMapCombine을 나타내는 Widget인거고
	//ClueMapWidget에서 선을 나타내면 끝이니까.

public:
	FORCEINLINE bool GetIsFoundCombinedClue() const { return bIsFoundCombinedClue; };
	FORCEINLINE void SetIsFoundCombinedClue(bool newIsFound) { bIsFoundCombinedClue = newIsFound; };
private:
	bool bIsFoundCombinedClue = false;

	UFUNCTION()
	void OnCobminedClueButtonClicked();
};
