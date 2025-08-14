// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stress/SRStressBar.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Subsystem/SRStressLocalPlayerSubsystem.h"

void USRStressBar::NativeConstruct()
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
            StressSubsystem->OnStressChangeByTimeDelegate.AddUObject(this, &ThisClass::OnStressWillChangeSoon);
      }
}

void USRStressBar::UpdateStressProgressBar(float StressAmount)
{
      //UE_LOG(LogTemp, Warning, TEXT("Stress Bar Update"));

      StackedStressBar->SetPercent(StressSubsystem->GetStressPercent());
}

void USRStressBar::OnStressWillChangeSoon(float ChangeValue, float DelayTime)
{
      PendingLerpDuration = DelayTime;

      // 현재 Stress 값과 앞으로 추가될 값
      const float CurrentStress = StressSubsystem->GetStress();
      const float FutureStress = FMath::Clamp(CurrentStress + ChangeValue, 0.f, 100.f); // 예: 95

      //미리 PreviewBar를 채움.
      //UE_LOG(LogTemp, Warning, TEXT("Stress Preview Bar Percent : %f"), FutureStress/100.0f);
      StressPreviewBar->SetPercent(FutureStress / 100.0f);

      PendingLerpTime = 0.0f;
      bIsPendingLerp = true;
}

void USRStressBar::StressChangePreviewColorChange(float InDeltaTime)
{
      if (!bIsPendingLerp) return;

      PendingLerpTime += InDeltaTime;
      float Alpha = FMath::Clamp(PendingLerpTime / PendingLerpDuration, 0.f, 1.f);
      //UE_LOG(LogTemp, Warning, TEXT("StressBar Native Tick Alpha : %f"), Alpha);

      // 점진적으로 붉은색으로 보간
      float R = FMath::Lerp(0.4f, 1.0f, Alpha); // 더 빨강으로
      float G = FMath::Lerp(0.4f, 0.0f, Alpha); // 점점 녹색 감소
      float B = FMath::Lerp(0.4f, 0.0f, Alpha); // 점점 파랑 감소

      FLinearColor FillColor = FLinearColor(R, G, B, 1.0f);

      //UE_LOG(LogTemp, Warning, TEXT("FillColor: R=%f, G=%f, B=%f, A=%f"), FillColor.R, FillColor.G, FillColor.B, FillColor.A);

      StressPreviewBar->SetFillColorAndOpacity(FillColor);
         
      if (Alpha >= 1.0f)
      {
            bIsPendingLerp = false;
      }
}

