// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyRoomPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ReadyRoomGameStateBase.h"
#include "Components/PanelWidget.h"
#include "Components/Widget.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "../Common/MyGameInstance.h"
#include "GameFramework/Character.h"

AReadyRoomPlayerController::AReadyRoomPlayerController()
{
	_bChattable = true;
}

void AReadyRoomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("Chat"), IE_Pressed, this, &AReadyRoomPlayerController::ShowChattingPannel);
	
}


void AReadyRoomPlayerController::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

void AReadyRoomPlayerController::Initialize()
{
	AGameStateBase* gameState = UGameplayStatics::GetGameState(GetWorld());
	_gameState = Cast<AReadyRoomGameStateBase>(gameState);
	_gameInstance = Cast<UMyGameInstance>(GetGameInstance());
	

	if (IsLocalPlayerController())
	{
		if (_gameInstance->_ReadyHUDAsset)
		{
			if (!_ReadyHUDOverlay)
			{
				_ReadyHUDOverlay = CreateWidget<UUserWidget>(this, _gameInstance->_ReadyHUDAsset);
				_ReadyHUDOverlay->SetVisibility(ESlateVisibility::Visible);
				_ReadyHUDOverlay->AddToViewport();
			}
		}

		if (_gameInstance->_ChattingHUDAsset)
		{
			if (!_ChattingHUDOverlay)
			{
				_ChattingHUDOverlay = CreateWidget<UUserWidget>(this, _gameInstance->_ChattingHUDAsset);
				_ChattingHUDOverlay->SetVisibility(ESlateVisibility::Hidden);
				_chatBox = Cast<UEditableText>(_ChattingHUDOverlay->GetWidgetFromName(FName("chattingBox")));
				_scrollbox = Cast<UScrollBox>(_ChattingHUDOverlay->GetWidgetFromName(FName("Scroller")));
				_ChattingHUDOverlay->AddToViewport();
				_chattingPannel = _ChattingHUDOverlay->GetRootWidget();
			}
		}

		if (HasAuthority())
		{
			if (_gameInstance->_StartHUDAsset)
			{
				if (!_StartHUDOverlay)
				{
					_StartHUDOverlay = CreateWidget<UUserWidget>(this, _gameInstance->_StartHUDAsset);
					_StartHUDOverlay->SetVisibility(ESlateVisibility::Visible);
					_StartHUDOverlay->AddToViewport();
				}
			}

			GetWorld()->GetTimerManager().SetTimer(_timerHandle2, FTimerDelegate::CreateLambda([&]()
				{
					if (_StartHUDOverlay)
					{
						if (_gameState->_bstartable)
						{
							
							if (_StartHUDOverlay->GetVisibility() == ESlateVisibility::Hidden)
									_StartHUDOverlay->SetVisibility(ESlateVisibility::Visible);
						}
						else
						{
							if (_StartHUDOverlay->GetVisibility() == ESlateVisibility::Visible)
								_StartHUDOverlay->SetVisibility(ESlateVisibility::Hidden);
						}
					}
				}
			), 0.5f, true, 0);
		}
	}
	_bChattable = true;
}



void AReadyRoomPlayerController::ShowChattingPannel()
{
	this->bShowMouseCursor = false;
	if(!_ChattingHUDOverlay)
	{
		return;
	}

	if (_toggle == false && _bChattable)
	{
		ShowChatBox();
		GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
		FSlateApplication::Get().SetKeyboardFocus(_chatBox->TakeWidget());
		_toggle = true;
	}
}

void AReadyRoomPlayerController::ShowChatBox()
{
	if (!_ChattingHUDOverlay)
	{
		return;
	}
	else if(_ChattingHUDOverlay->GetVisibility() == ESlateVisibility::Hidden)
	{
		_ChattingHUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}

	if(_chattingPannel)
		_chattingPannel->GetCachedWidget()->SetRenderOpacity(1);
}



void AReadyRoomPlayerController::commit_Implementation(const FString& message)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Commit in ReadyRoom!"));

	if (!message.IsEmpty())
	{
		_messageSent = _gameInstance->_characterInfo._CharacterName + FString(": ") + message;
	}
	else
	{
		_messageSent = "";
	}
	

	CreateTextHistoryFromServer(_messageSent);
	
	_toggle = false;

	
	//Only for fading effect
	

	GetWorldTimerManager().SetTimer(_timerHandle, FTimerDelegate::CreateLambda([&]()
	{
		_chattingPannel->GetCachedWidget()->SetRenderOpacity(_chattingPannel->GetCachedWidget()->GetRenderOpacity()-0.05);
		if (_chattingPannel->GetCachedWidget()->GetRenderOpacity()<0)
		{
			GetWorldTimerManager().ClearTimer(_timerHandle);
		}
	}), 0.3, true, 0.3);

	
}


void AReadyRoomPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AReadyRoomPlayerController::CreateTextHistoryFromServer_Implementation(const FString& _message)
{
	if (_message.IsEmpty())
	{
		return;
	}
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetPawn()->GetWorld(), APlayerController::StaticClass(), OutActors);
	for (AActor* OutActor : OutActors)
	{
		AReadyRoomPlayerController* playerController = Cast<AReadyRoomPlayerController>(OutActor);
		if (playerController)
		{
			playerController->makeHistoryFromClient(_message);
		}
	}	
}

void AReadyRoomPlayerController::makeHistoryFromClient_Implementation(const FString& _message)
{
	ShowChatBox();

	UTextBlock* NewTextBlock = NewObject<UTextBlock>();

	NewTextBlock->Font.Size = 15;
	NewTextBlock->SetText(FText::FromString(_message));
	
	_scrollbox->AddChild(NewTextBlock);
	_scrollbox->ScrollToEnd();
	
}

