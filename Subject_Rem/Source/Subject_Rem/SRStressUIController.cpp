// Fill out your copyright notice in the Description page of Project Settings.


#include "SRStressUIController.h"
#include "Subsystem/SRStressLocalPlayerSubsystem.h"
#include "Components/ProgressBar.h"

void USRStressUIController::NativeConstruct()
{
      Super::NativeConstruct();

      if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
      {
            StressSubsystem = LocalPlayer->GetSubsystem<USRStressLocalPlayerSubsystem>();
            if (!StressSubsystem)
            {
                  UE_LOG(LogTemp, Warning, TEXT("Stress Subsystem not found"));
                  return;
            }
            check(StressSubsystem);

            StressSubsystem->OnStressChangeDelegate.AddUObject(this, &ThisClass::UpdateStressProgressBar);
      }
}

void USRStressUIController::UpdateStressProgressBar(float StressAmount)
{
      check(StressSubsystem);

      UE_LOG(LogTemp, Log, TEXT("StressProgressBar Update"));
      float StressPercent = StressSubsystem->GetStressPercent();

      // ProgressBar에 퍼센트 적용
      if (StressProgressBar)
      {
            StressProgressBar->SetPercent(StressPercent);

            // G, B는 0 ~ 0.05 사이에서 감소
            float GBAlpha = FMath::Clamp(StressPercent / 0.8f, 0.0f, 1.0f);
            float G = FMath::Lerp(0.05f, 0.0f, GBAlpha);
            float B = FMath::Lerp(0.05f, 0.0f, GBAlpha);

            // R은 고정값 (빨강 강조)
            FLinearColor FillColor = FLinearColor(0.4f, G, B, 1.0f);
            StressProgressBar->SetFillColorAndOpacity(FillColor);
      }
}
