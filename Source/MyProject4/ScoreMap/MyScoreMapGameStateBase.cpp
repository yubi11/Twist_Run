// Fill out your copyright notice in the Description page of Project Settings.


#include "MyScoreMapGameStateBase.h"
#include "MyScoreMapPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"


AMyScoreMapGameStateBase::AMyScoreMapGameStateBase()
{
	PrimaryActorTick.bCanEverTick = true;
	_gameCount = 0;
	_leastNum = 3;
	_bstartable = false;
}

void AMyScoreMapGameStateBase::BeginPlay()
{
	Super::BeginPlay();

}

int AMyScoreMapGameStateBase::GetNumberOfUser()
{
	return PlayerArray.Num();
}


void AMyScoreMapGameStateBase::FindAllPlayerControllerHideAllWidget_Implementation()
{

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), OutActors);
	for (auto playerController : OutActors)
	{
		AMyScoreMapPlayerController* pc = Cast<AMyScoreMapPlayerController>(playerController);
		if (pc->_ChattingHUDOverlay)
		{
			pc->_ChattingHUDOverlay->RemoveFromParent();
		}
		if (pc->_ScoreHUDOverlay)
		{
			pc->_ScoreHUDOverlay->RemoveFromParent();
		}
		pc->GetWorldTimerManager().ClearTimer(pc->_timerHandle);
		pc->GetWorldTimerManager().ClearTimer(pc->_timerHandle2);
	}

	HideAllWidget();
}

void AMyScoreMapGameStateBase::HideAllWidget_Implementation()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AMyScoreMapPlayerController* pc = Cast<AMyScoreMapPlayerController>(playerController);

	if (pc->_ChattingHUDOverlay)
	{
		pc->_ChattingHUDOverlay->RemoveFromParent();
	}
	if (pc->_ScoreHUDOverlay)
	{
		pc->_ScoreHUDOverlay->RemoveFromParent();
	}
	pc->GetWorldTimerManager().ClearTimer(pc->_timerHandle);
	pc->GetWorldTimerManager().ClearTimer(pc->_timerHandle2);
}







void AMyScoreMapGameStateBase::FindAllPlayerControllerOpenAllWidget_Implementation()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), OutActors);
	for (auto playerController : OutActors)
	{
		AMyScoreMapPlayerController* pc = Cast<AMyScoreMapPlayerController>(playerController);
		if (pc->_ChattingHUDOverlay)
		{
			pc->_ChattingHUDOverlay->SetVisibility(ESlateVisibility::Visible);
		}
	}
	OpenAllWidget();
}


void AMyScoreMapGameStateBase::OpenAllWidget_Implementation()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AMyScoreMapPlayerController* pc = Cast<AMyScoreMapPlayerController>(playerController);
	if (pc->_ChattingHUDOverlay)
	{
		pc->_ChattingHUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}
