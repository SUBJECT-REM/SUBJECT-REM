// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/SRInputLocalPlayerSubsystem.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

void USRInputLocalPlayerSubsystem::ApplyInputRestriction()
{
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(EnhancedInputSubsystem);

	if (EnhancedInputSubsystem)
	{
		//현재 입력매핑컨텍스트를 가져오고 , 캐싱 후 제거
		//EnhancedInputSubsystem->
		EnhancedInputSubsystem->ClearAllMappings(); 
	}
}

void USRInputLocalPlayerSubsystem::RemoveInputRestriction()
{

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(EnhancedInputSubsystem);
	if (CachedIMC.IsValid())
	{
		// 먼저 깨끗하게
		EnhancedInputSubsystem->ClearAllMappings();
		// 캐시했던 IMC를 원래 우선순위로 다시 적용
		EnhancedInputSubsystem->AddMappingContext(CachedIMC.Context, CachedIMC.Priority);
	}
}

void USRInputLocalPlayerSubsystem::ReplaceContext(UInputMappingContext* ReplaceContext, int32 Priority)
{
	check(ReplaceContext);

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(EnhancedInputSubsystem);

	if (EnhancedInputSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("replace Context"));
		EnhancedInputSubsystem->ClearAllMappings(); 
		EnhancedInputSubsystem->AddMappingContext(ReplaceContext, Priority);

		CachedIMC = FIMCEntry(ReplaceContext , Priority);
	}
}
