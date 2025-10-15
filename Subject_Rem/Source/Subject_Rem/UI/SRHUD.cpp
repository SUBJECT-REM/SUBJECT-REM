// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRHUD.h"
#include "Blueprint/UserWidget.h"


UUserWidget* ASRHUD::GetWidgetByClass(TSubclassOf<UUserWidget> WidgetClass, bool bExactMatch)
{
    if (!WidgetClass) return nullptr;

    for (UUserWidget* AddedWidget : AddedViewportWidgets)
    {
        if (!AddedWidget) continue;

        const bool bMatch = bExactMatch ? (AddedWidget->GetClass() == *WidgetClass)  :  AddedWidget->IsA(WidgetClass);              

        if (bMatch)
        {
            return AddedWidget;
        }
    }
    return nullptr;
}

void ASRHUD::BeginPlay()
{
	Super::BeginPlay();

	for (const FUIControllerInfo& UIControllerInfo : UIControllerInfos)
	{
		UUserWidget* NewUIController = CreateWidget<UUserWidget>(GetWorld(), UIControllerInfo.UIControllerClass);

		NewUIController->AddToViewport(UIControllerInfo.ZOrder);
		AddedViewportWidgets.Add(NewUIController);
	}
}
