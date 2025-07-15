// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SRPresenter.generated.h"

/**
 * Presenter 클래스는 
 * UI가 게임로직을 참조하지 않도록 하는 중간 클래스 역할을 합니다.
 */
UCLASS(Abstract)
class SUBJECT_REM_API USRPresenter : public UObject
{
	GENERATED_BODY()
	
public:
	/*게임 로직과 UI를 Presenter에서 소유하도록 Init함수를 구현합니다.*/
	virtual void Init(UActorComponent* InitComponent , UUserWidget* InitWidget);
};
