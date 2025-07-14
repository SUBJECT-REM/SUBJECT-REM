// Fill out your copyright notice in the Description page of Project Settings.


#include "SRStressUIController.h"
#include "Subsystem/SRStressLocalPlayerSubsystem.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

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

      SetHightStressEffectAlpha(0.0f);
}

void USRStressUIController::UpdateStressProgressBar(float StressAmount)
{
      check(StressSubsystem);

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

      //StressEffect 실행 및 멈춤
      if (StressPercent >= 0.8f)
      {
            if (!IsAnimationPlaying(Pulse))
            {
                  PlayDangerPulse(true);
            }
      }
      else
      {
            if (IsAnimationPlaying(Pulse))
            {
                  PlayDangerPulse(false);
                  SetHightStressEffectAlpha(0.0f);
            }
      }
}

void USRStressUIController::SetHightStressEffectAlpha(float Alpha)
{
      check(HighStressEffect);

      UE_LOG(LogTemp, Log, TEXT("StressProgressBar Alpha : %f"), Alpha);
      FLinearColor Color = HighStressEffect->ColorAndOpacity;
      Color.A = FMath::Clamp(Alpha, 0.0f, 1.0f);
      HighStressEffect->SetColorAndOpacity(Color);
}

void USRStressUIController::PlayDangerPulse(bool bIsPlay)
{
      check(Pulse);

      if (bIsPlay)
      {
            //재생이 끊겨서 다시 재생되는 현상 방지
            if (!IsAnimationPlaying(Pulse))
            {
                  HighStressEffect->SetVisibility(ESlateVisibility::Visible);
                  PlayAnimation(Pulse, 0.f, -1);
            }
      }
      else 
      { 
            StopAnimation(Pulse);
            HighStressEffect->SetVisibility(ESlateVisibility::Hidden);
            SetHightStressEffectAlpha(0.0f);
      }
      
}
