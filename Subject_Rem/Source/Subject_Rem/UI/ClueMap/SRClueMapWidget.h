// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRItemData.h"
#include "SRClueMapWidget.generated.h"

class UCanvasPanel;
class USRInventoryComponent;
class USRClueMapCombinedResultWidget;

UCLASS()
class SUBJECT_REM_API USRClueMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	//조합된 Clue들은 해당 Canvas 아래에 넣으면 됩니다.
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* ClueMapCanvas;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* ClueLinkLineCanvas;

protected:
	bool bIsInitialized = false;


	//ClueMap
	//최초에 한번만 실행하며 CombinedClueWidget을 찾습니다.
	void FindCombinedResultWidgets();
	void FindCombinedClueResultWidget(FName CombinedClueName);
	UFUNCTION()
	void HandleCombinedClue(const FSRClueMapData& Data);

	void FindPlayerInventoryComponent();
	void BindInventoryDelegate();
	//현재 사용하지 않음.
	void UnbindInventoryDelegate();

	//Widget들만 모아서 미리 찾아 Array에 넣는 Array
	TMap<FName, USRClueMapCombinedResultWidget*> CombinedClueWidgets;
	//FName으로 찾아 해당 조합된 USRClueMapCombinedResultWidget를 얻습니다. - 진실, 거짓 상관없이 다 담습니다.
	TMap<FName, USRClueMapCombinedResultWidget*> FoundCombinedClueWidgets;
	//진실 단서만 담습니다.
	TArray<FName> TrueClues;

private:
	USRInventoryComponent* InventoryComponent;
};
