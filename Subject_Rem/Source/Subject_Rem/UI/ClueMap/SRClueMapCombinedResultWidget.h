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

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* LayoutSwitcher;

	//Descrtion을 위한 Button
	UPROPERTY(meta = (BindWidget))
	UButton* ClueMapDesShownButton;
	//Button Image; 
	UPROPERTY(meta = (BindWidget))
	UImage* LeftClueImage;
	UPROPERTY(meta = (BindWidget))
	UImage* RightClueImage;

	UPROPERTY(meta = (BindWidget))
	UImage* LeftClueImage_3;

	UPROPERTY(meta = (BindWidget))
	UImage* MidImage_3;

	UPROPERTY(meta = (BindWidget))
	UImage* RightClueImage_3;
	

	FClueMapCombinedResultClickedSignature ClueMapCombinedResultClickedDelegate;

public:
	FORCEINLINE bool GetIsFoundCombinedClue() const { return bIsFoundCombinedClue; };
	FORCEINLINE void SetIsFoundCombinedClue(bool newIsFound) { bIsFoundCombinedClue = newIsFound; };
private:
	bool bIsFoundCombinedClue = false;

	UFUNCTION()
	void OnCobminedClueButtonClicked();
};
