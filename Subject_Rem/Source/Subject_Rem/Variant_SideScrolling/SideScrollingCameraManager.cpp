// Copyright Epic Games, Inc. All Rights Reserved.


#include "SideScrollingCameraManager.h"
#include "GameFramework/Pawn.h"
#include "Engine/HitResult.h"
#include "CollisionQueryParams.h"
#include "Engine/World.h"

void ASideScrollingCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	//ViewTarget이 Pawn인지 확인
	APawn* TargetPawn = Cast<APawn>(OutVT.Target);

	// is our target valid?
	if (IsValid(TargetPawn))
	{
		// set the view target FOV and rotation
		//OutVT.POV.Rotation = FRotator(0.0f, -90.0f, 0.0f);
		OutVT.POV.Rotation = Rotation;
		OutVT.POV.FOV = 65.0f;

		//현재 타겟 위치와 카메라 위치 저장
		FVector CurrentActorLocation = OutVT.Target->GetActorLocation();
		FVector CurrentCameraLocation = GetCameraLocation();

		// Y축 위치를 X축 기준으로 계산 (정면 카메라를 위해)
		float CurrentX, CurrentY;

		if (ShowX)
		{
			CurrentX = -(CurrentZoom + CurrentActorLocation.X);
		}
		else
		{
			CurrentY = CurrentZoom + CurrentActorLocation.Y;
		}
		

		// do first-time setup
		if (bSetup)
		{
			// lower the setup flag
			bSetup = false;
			 
			if (ShowX)
			{
				// initialize the camera viewpoint and return
				OutVT.POV.Location.X = CurrentX;
				OutVT.POV.Location.Y = CurrentActorLocation.Y;
				OutVT.POV.Location.Z = CurrentActorLocation.Z + CameraZOffset;
			}
			else
			{
				OutVT.POV.Location.X = CurrentActorLocation.X;
				OutVT.POV.Location.Y = CurrentY;
				OutVT.POV.Location.Z = CurrentActorLocation.Z + CameraZOffset;
			}


			// save the current camera height
			CurrentZ = OutVT.POV.Location.Z;

			// skip the rest of the calculations
			return;
		}

		// check if the camera needs to update its height
		bool bZUpdate = false;

		// is the character moving vertically?
		if (FMath::IsNearlyZero(TargetPawn->GetVelocity().Z))
		{
			// determine if we need to do a height update
			bZUpdate = FMath::IsNearlyEqual(CurrentZ, CurrentCameraLocation.Z, 25.0f);

		} else {

			// run a trace below the character to determine if we need to do a height update
			FHitResult OutHit;

			const FVector End = CurrentActorLocation + FVector(0.0f, 0.0f, -1000.0f);

			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(TargetPawn);

			// only update height if we're not about to hit ground
			bZUpdate = !GetWorld()->LineTraceSingleByChannel(OutHit, CurrentActorLocation, End, ECC_Visibility, QueryParams);

		}

		// do we need to do a height update?
		if (bZUpdate)
		{

			// set the height goal from the actor location
			CurrentZ = CurrentActorLocation.Z;

		} else {

			// are we close enough to the target height?
			if (FMath::IsNearlyEqual(CurrentZ, CurrentActorLocation.Z, 100.0f))
			{
				// set the height goal from the actor location
				CurrentZ = CurrentActorLocation.Z;

			} else {

				// blend the height towards the actor location
				CurrentZ = FMath::FInterpTo(CurrentZ, CurrentActorLocation.Z, DeltaTime, ZCameraLocationUpdateSpeed);
				
			}

		}

		// clamp the X axis to the min and max camera bounds
		if (ShowX)
		{
			CurrentY = FMath::Clamp(CurrentActorLocation.Y, CameraYMinBounds, CameraYMaxBounds);
		}
		else
		{
			CurrentX = FMath::Clamp(CurrentActorLocation.X, CameraXMinBounds, CameraXMaxBounds);
		}

		// blend towards the new camera location and update the output
		//FVector TargetCameraLocation(CurrentX, CurrentY, CurrentZ);
		FVector TargetCameraLocation(CurrentX, CurrentY, CurrentZ);

		OutVT.POV.Location = FMath::VInterpTo(CurrentCameraLocation, TargetCameraLocation, DeltaTime, CameraLocationUpdateSpeed);
	}
}
