// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyWorldTimer.generated.h"

/**
 * 
 */

/*
FTimerHandle WaitHandle;
float WaitTime; //�ð��� �����ϰ�
GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
	{
		// ���⿡ �ڵ带 ġ�� �ȴ�.
	}), WaitTime, false); //�ݺ��� ���⼭ �߰� ������ ������ ��������
		delay �����Լ� 
*/

UCLASS()
class MYPROJECT4_API AMyWorldTimer : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyWorldTimer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaTime) override;


	UFUNCTION(Server, BlueprintCallable,Reliable)
	void NotifyArrival();
	
	

	UFUNCTION(Server, Unreliable)
	void Spectate();

	UFUNCTION(Server, Unreliable)
	void ChangeView();

	UFUNCTION(Server, Reliable)
		void AllMightyMode();


	UFUNCTION(Server, Unreliable)
	void UpdateHUD();


	UFUNCTION(Client, Unreliable)
		void BrushSockets(int index, uint8 ept);

	UFUNCTION(Client, Unreliable)
		void ClearSockets(int index);

	UFUNCTION(Client, Unreliable)
		void TurnOnSpectateUI();

	UFUNCTION(Client, Unreliable)
		void TurnOnFinalTimerUI();

	UFUNCTION(Server, Unreliable)
		void SetCharacterName(const FString& name);




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TArray<FSlateBrush> _sockImages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		FSlateBrush _defaultImage;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* _FinalTimerHUDOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* _SpectateHUDOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* _inventoryHUDOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* _GameOverHUDOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* _StartTimerHUDOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		class AMyCharacter* _character;


	TArray<APlayerState*> _playerArray;


	AController* _previous;

	class TArray<AActor*> _actors;

	class UMyGameInstance* _gameInstance;

	class AMyPlayerState* _playerState;
	
	class AMyGameStateBase* _gameState;

	class AMySpectatorPawn* _spectatorPawn;

	class AMyProject4GameModeBase* _gameMode;

	class AMyWorldTimer* _currentSpectated;

	int _currentIndex;


	FTimerHandle UpdateHandle;

	FTimerHandle DelayHandle;


	//AMyActorSubClass* MyActor = GetWorld()->SpawnActor<AMyActorSubClass>(AMyActorSubClass::StaticClass());
};
