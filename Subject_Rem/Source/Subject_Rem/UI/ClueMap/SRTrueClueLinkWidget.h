// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRTrueClueLinkWidget.generated.h"

/**
 * 
 */
class UButton;
UCLASS()
class SUBJECT_REM_API USRTrueClueLinkWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterSelect")
	FName CombinedClueID;
	//이미지 호버링때문에 편하게 버튼 사용함.
	UPROPERTY(EditAnywhere, meta = (BindWidget),BlueprintReadWrite)
	UButton* LinkButtonImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* NormalImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* HoverImage;
private:

};
