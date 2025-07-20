// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRRotateItemPreviewWidget.generated.h"

class UImage;
class ASRSmoothRotateActor;
class USRRotateableStaticMeshComponent;

UCLASS()
class SUBJECT_REM_API USRRotateItemPreviewWidget : public UUserWidget
{
	GENERATED_BODY()
protected:	
	//위젯이 켜질 때
	virtual void NativeConstruct() override;
	//위젯이 꺼졌을 때
	virtual void NativeDestruct() override;

	//마우스 클릭 시작
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//마우스를 움직일 때 회전값 계산
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//마우스를 떼면 드레그 종료
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//마우스가 UI를 나가면 실행.
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	bool bDragging = false;
	FVector2D LastMousePosition;

	UPROPERTY()
	USRRotateableStaticMeshComponent* RotateableMeshComp;

	UPROPERTY(meta = (BindWidget))
	UImage* PreviewImage;

private: 
	void GetTargetActorFromLevel();

public:
	//void SetTargetActor(ASRSmoothRotateActor* InActor) { TargetActor = InActor; }
};
