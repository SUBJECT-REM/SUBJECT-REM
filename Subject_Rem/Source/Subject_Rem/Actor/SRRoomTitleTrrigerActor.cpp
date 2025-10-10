// Fill out your copyright notice in the Description page of Project Settings.


#include "SRRoomTitleTrrigerActor.h"
#include "Components/BoxComponent.h"
#include "UI/SRHUD.h"
#include "UI/SRRoomTitleWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASRRoomTitleTrrigerActor::ASRRoomTitleTrrigerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxCollisionBeginOverlap);

}

// Called when the game starts or when spawned
void ASRRoomTitleTrrigerActor::BeginPlay()
{
	Super::BeginPlay();

	GameFlowManager = Cast<ASRGameFlowManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ASRGameFlowManager::StaticClass()));
	
}

void ASRRoomTitleTrrigerActor::OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AController* InstigatorController = OtherActor->GetInstigatorController();
	APlayerController* PC = nullptr;
	ASRHUD* SRHUD = nullptr;

	if (!InstigatorController)
		return;

	PC = Cast<APlayerController>(InstigatorController);
	if (!PC)
		return;
	
	SRHUD = PC->GetHUD<ASRHUD>();
	if (!SRHUD)
		return;

	FVector PlayerDir = OtherActor->GetActorForwardVector();
	FVector TriggerDir = GetActorForwardVector();

	float Dot = FVector::DotProduct(PlayerDir.GetSafeNormal2D(), TriggerDir.GetSafeNormal2D());

	FRoomTitleInfo SelectedRoomTitleInfo = (Dot >= 0.f) ? LeftToRightTitleInfo : RightToLeftTitleInfo;

	USRRoomTitleWidget* RoomTitleWidget =SRHUD->GetWidgetOfClass<USRRoomTitleWidget>(USRRoomTitleWidget::StaticClass());
	if (RoomTitleWidget)
	{
		if (RoomTitleWidget)
		{
			RoomTitleWidget->SetRoomTitleImage(SelectedRoomTitleInfo.Image);
			RoomTitleWidget->SetRoomTitleImageSize(SelectedRoomTitleInfo.Size);
			RoomTitleWidget->ShowRoomTitleWidget();
		}
	}
	
	if (bTriggerOnce)
	{
		BoxCollision->SetGenerateOverlapEvents(false);
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BoxCollision->OnComponentBeginOverlap.RemoveDynamic(
			this, &ThisClass::OnBoxCollisionBeginOverlap);
	}
	
	if (GameFlowManager.IsValid() && ApplyRoomFlowInfo)
	{
		GameFlowManager->SequenceFlowInfos.Add(RoomGameFlowInfo);
		GameFlowManager->StartFlow();
		ApplyRoomFlowInfo = false;
	}
}



