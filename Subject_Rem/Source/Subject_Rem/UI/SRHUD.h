// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "SRHUD.generated.h"

USTRUCT()
struct FUIControllerInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> UIControllerClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int32 ZOrder = 0;
};

UCLASS()
class SUBJECT_REM_API ASRHUD : public AHUD
{
	GENERATED_BODY()
public:
	template<class T>
	T* GetWidgetOfClass(TSubclassOf<UUserWidget> ExactClass);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI", meta = (DeterminesOutputType = "WidgetClass"))
	UUserWidget* GetWidgetByClass(TSubclassOf<UUserWidget> WidgetClass, bool bExactMatch);

	UPROPERTY(BlueprintReadOnly)
	TArray<UUserWidget*> AddedViewportWidgets;
private:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TArray<FUIControllerInfo> UIControllerInfos;

};

template<class T>
 T* ASRHUD::GetWidgetOfClass(TSubclassOf<UUserWidget> ExactClass)
{
	for (UUserWidget* AddedWidget : AddedViewportWidgets)
	{
		if (AddedWidget && AddedWidget->IsA(ExactClass))          // 정확히 일치
			return Cast<T>(AddedWidget);
	}
	return nullptr;
}
