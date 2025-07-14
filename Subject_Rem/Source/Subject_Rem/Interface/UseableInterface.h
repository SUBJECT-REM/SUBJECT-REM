// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UseableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUseableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SUBJECT_REM_API IUseableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//어떤 효과를 부여할지 재정의하시면 됩니다.
	virtual void UseEffect() = 0;
};
