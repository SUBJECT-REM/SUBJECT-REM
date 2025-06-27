// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "SideScrollingCameraManager.generated.h"

/**
 *  Simple side scrolling camera with smooth scrolling and horizontal bounds
 */
UCLASS()
class ASideScrollingCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:

	/** Overrides the default camera view target calculation */
	/*@param OutVT : 카메라가 따라다니는 Target
	* @param DeltaTime : 자연스러운 움직임을 위한 보간값
	*/
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

public:
	/*얼마나 Target과 가까이있는가? - 값이 작을 수록 가까이 있음.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Side Scrolling Camera", meta=(ClampMin=0, ClampMax=10000, Units="cm"))
	float CurrentZoom = 1000.0f;
	/** 최초 카메라 값. 초기화 이후엔 사용하지 않습니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Side Scrolling Camera", meta=(ClampMin=0, ClampMax=10000, Units="cm"))
	float CameraZOffset = 100.0f;
	/** Minimum camera scrolling bounds in world space */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Side Scrolling Camera", meta=(ClampMin=0, ClampMax=10000, Units="cm"))
	float CameraXMinBounds = -400.0f;
	/** Maximum camera scrolling bounds in world space */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Side Scrolling Camera", meta=(ClampMin=0, ClampMax=10000, Units="cm"))
	float CameraXMaxBounds = 10000.0f;

	//카메라 세로이동(높이) 보간 속도 - 값이 클수록 Target에게 빠르게 감.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Side Scrolling Camera", meta = (ClampMin = 0, ClampMax = 10, Units = "cm"))
	float ZCameraLocationUpdateSpeed = 2.0f;
	//카메라 이동 보간 속도 - 값이 클수록 Target에게 빠르게 감.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Side Scrolling Camera", meta = (ClampMin = 0, ClampMax = 10, Units = "cm"))
	float CameraLocationUpdateSpeed = 2.0f;


protected:

	/** Last cached camera vertical location. The camera only adjusts its height if necessary. */
	float CurrentZ = 0.0f;
	/** First-time update camera setup flag */
	bool bSetup = true;
};
