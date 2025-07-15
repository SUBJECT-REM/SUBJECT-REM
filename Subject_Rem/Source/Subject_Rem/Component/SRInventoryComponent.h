// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SRClueData.h"
#include "SRInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeClueDatasSignatue,const FSRClueData&, Data);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUBJECT_REM_API USRInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USRInventoryComponent();
	
	/*단서 데이터 추가*/
	UFUNCTION(BlueprintCallable)
	void AddClueData(const FSRClueData& Data);

	/*ClueDatas 변경에 대한 델리게이트*/
	FChangeClueDatasSignatue ChangeClueDatasDelegate;
private:	
	/*소유하고 있는 단서 데이터들*/
	UPROPERTY(EditDefaultsOnly)
	TArray<FSRClueData> ClueDatas;
		
};
