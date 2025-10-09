// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRItem.h"
#include "SRClueMapCombinedResultWidget.generated.h"

class UButton;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FClueMapCombinedResultClickedSignature, const FName&, Id);

UCLASS()
class SUBJECT_REM_API USRClueMapCombinedResultWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;   

	virtual void NativeConstruct() override;

	void UpdateClueImage(TArray<TSoftObjectPtr<UTexture2D>> Icons);

	void SetClueMapId(FName Name) { CombinedClueID = Name; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	UDataTable* ClueDataTable;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 ExpectedIconCount = 2; // 2 또는 3

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FButtonStyle ButtonThreeStyle;
private:
	bool bIsFoundCombinedClue = false;

	UFUNCTION()
	void OnCobminedClueButtonClicked();

	void SetImageBrush(UImage* Image, const TSoftObjectPtr<UTexture2D> SoftTex);

	//해당 ClueID로 어떤 Clue인지 확인합니다.
	UPROPERTY()
	FName CombinedClueID;



};
