// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRHUD.h"
#include "Blueprint/UserWidget.h"

void ASRHUD::BeginPlay()
{
	Super::BeginPlay();

	for (const FUIControllerInfo& UIControllerInfo : UIControllerInfos)
	{
		UUserWidget* NewUIController = CreateWidget<UUserWidget>(GetWorld(), UIControllerInfo.UIControllerClass);

		NewUIController->AddToViewport(UIControllerInfo.ZOrder);
	}
}
