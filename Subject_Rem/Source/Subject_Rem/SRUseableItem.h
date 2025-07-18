// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SRItem.h"
#include "Interface/UseableInterface.h"
#include "SRUseableItem.generated.h"

/**
 * 
 */
UCLASS()
class SUBJECT_REM_API USRUseableItem : public USRItem, public IUseableInterface
{
	GENERATED_BODY()
	
public:
	virtual void UseEffect() override;

};
