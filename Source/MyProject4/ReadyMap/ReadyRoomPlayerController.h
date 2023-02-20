// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Interface/MyChattingControllerInterface.h"
#include "ReadyRoomPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT4_API AReadyRoomPlayerController : public APlayerController, public IMyChattingControllerInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AReadyRoomPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

	void Initialize();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Chatting")
	void commit(const FString& message);
	virtual void commit_Implementation(const FString& message);

	virtual void ShowChattingPannel();

	UFUNCTION(BlueprintCallable, Category = "Chat")
		virtual void ShowChatBox();

	UFUNCTION(Server, Reliable)
		virtual void CreateTextHistoryFromServer(const FString& _message);

	UFUNCTION(Client, Reliable)
		virtual void makeHistoryFromClient(const FString& _message);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* _ReadyHUDOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* _ChattingHUDOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* _StartHUDOverlay;

	class AReadyRoomGameStateBase* _gameState;

	class UMyGameInstance* _gameInstance;

	bool _bChattable;

	bool _toggle;

	class UEditableText* _chatBox;

	class UWidget* _chattingPannel;

	class UScrollBox* _scrollbox;
	
	FString _messageSent;

	FTimerHandle _timerHandle;
	FTimerHandle _timerHandle2;
};
