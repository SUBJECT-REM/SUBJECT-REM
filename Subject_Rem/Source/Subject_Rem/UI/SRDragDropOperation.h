// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SRItemData.h"
#include "SRDragDropOperation.generated.h"

/**
 * 
 */

class USRSlotWidget;

UCLASS()
class SUBJECT_REM_API USRDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	void MoveToSlotData(USRSlotWidget* Drop);

	UPROPERTY()
	USRSlotWidget* DraggedSlot;

};
