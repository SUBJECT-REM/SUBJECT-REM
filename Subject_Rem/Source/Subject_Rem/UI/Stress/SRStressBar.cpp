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
      }
      CollectProgressBars();
}

void USRStressBar::UpdateStressProgressBar(float StressAmount)
{
      UE_LOG(LogTemp, Warning, TEXT("Stress Bar Update"));
      // 가득 찬 Block 개수 계산
      const int32 FullBlocks = FMath::FloorToInt(StressAmount / 10.0f);
      // 마지막 Block의 채움 비율 (0.0 ~ 0.9)
      const float PartialPercent = FMath::Fmod(StressAmount, 10.0f) / 10.0f;

      for (int32 i = 0; i < ProgressBarArray.Num(); ++i)
      {
            if (!ProgressBarArray[i]) continue;

            float Fill = 0.f;

            if (i < FullBlocks)
            {
                  // 가득
                  Fill = 1.0f; 
            }
            else if (i == FullBlocks)
            {
                  //특정 비율
                  Fill = PartialPercent; 
            }
            else
            {
                  // 비움
                  Fill = 0.0f;
            }

            ProgressBarArray[i]->SetPercent(Fill);
      }
}

void USRStressBar::CollectProgressBars()
{
      if (!HorizontalBox) return;

      const int32 Count = HorizontalBox->GetChildrenCount();
      for (int32 i = 0; i < Count; ++i)
      {
            if (UWidget* Child = HorizontalBox->GetChildAt(i))
            {
                  if (UProgressBar* Border = Cast<UProgressBar>(Child))
                  {
                        ProgressBarArray.Add(Border);
                  }
            }
      }
}

float USRStressBar::GetHealthPercent()
{
      check(StressSubsystem);
	return StressSubsystem->GetStressPercent();;
}
