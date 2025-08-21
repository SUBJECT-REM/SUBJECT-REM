// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/SRInputLocalPlayerSubsystem.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

void USRInputLocalPlayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("InputSubSystemInit"));
	Collection.InitializeDependency(UEnhancedInputLocalPlayerSubsystem::StaticClass());
}

void USRInputLocalPlayerSubsystem::LockInput()
{
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(EnhancedInputSubsystem);

	if (EnhancedInputSubsystem)
	{
		EnhancedInputSubsystem->ClearAllMappings(); 
	}
}

void USRInputLocalPlayerSubsystem::UnlockInput()
{

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(EnhancedInputSubsystem);

	// 캐시가 있으면 캐시를, 없으면 기본 세트를 복원
	const TArray<FIMCEntry>& ToRestore = (CachedIMCs.Num() > 0) ? CachedIMCs : DefaultContexts;

	for (const FIMCEntry& E : ToRestore)
	{
		if (E.IsValid())
		{
			EnhancedInputSubsystem->AddMappingContext(E.Context, E.Priority);
		}
	}
}

void USRInputLocalPlayerSubsystem::ReplaceContexts(const TArray<FIMCEntry>& NewContexts)
{
	auto* EI = GetEnhancedChecked();

	// 통째로 교체
	EI->ClearAllMappings();

	for (const FIMCEntry& Context : NewContexts)
	{
		if (Context.IsValid())
		{
			EI->AddMappingContext(Context.Context, Context.Priority);
			CachedIMCs.Add(Context);
		}
	}
}

void USRInputLocalPlayerSubsystem::SetDefaultContexts(const TArray<FIMCEntry>& Contexts)
{
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(EnhancedInputSubsystem);

	for (FIMCEntry CurrentContext : Contexts)
	{
		EnhancedInputSubsystem->AddMappingContext(CurrentContext.Context, CurrentContext.Priority);
		CachedIMCs.Add(CurrentContext);
		DefaultContexts.Add(CurrentContext);
	}
}

UEnhancedInputLocalPlayerSubsystem* USRInputLocalPlayerSubsystem::GetEnhancedChecked() const
{
	ULocalPlayer* LP = GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* EI = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP);
	check(EI);

	return EI;
}
