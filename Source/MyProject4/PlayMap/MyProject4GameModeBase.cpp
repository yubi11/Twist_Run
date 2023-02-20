
#include "MyProject4GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "../ReadyMap/ReadyRoomGameStateBase.h"
#include "Blueprint/UserWidget.h"
#include "../ReadyMap/ReadyRoomGameStateBase.h"
#include "../ReadyMap/ReadyRoomPlayerState.h"
#include "MyWorldTimer.h"
#include "MyGameStateBase.h"
#include "../Common/MyCharacter.h"
#include "MyEnemy.h"
#include "AIController.h"
#include "../Common/MyGameInstance.h"
#include "MySpectatorPawn.h"

AMyProject4GameModeBase::AMyProject4GameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerControllerClass = AMyWorldTimer::StaticClass();
	GameStateClass = AMyGameStateBase::StaticClass();
	PlayerStateClass = APlayerState::StaticClass();
	DefaultPawnClass = AMyCharacter::StaticClass();
	SpectatorClass = AMySpectatorPawn::StaticClass();

	_AIRespawnPoint.X = 500;
	_AIRespawnPoint.Y = 500;
	_AIRespawnPoint.Z += 90;

	_NumOfFinished = 0;
	_bossTimer = 120;
	_finalTimer = 20;
	_bBoss = true;
	_bIsFinal = false;
	_bIsStarted = false;

	_StartTimer = 5;

}

// Called when the game starts or when spawned
void AMyProject4GameModeBase::BeginPlay()
{
	Super::BeginPlay();
	_gameState = GetGameState<AMyGameStateBase>();

}

void AMyProject4GameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CountTime(DeltaTime);
}


void AMyProject4GameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	if ((Cast<UMyGameInstance>(GetGameInstance())->_gameStarted))
	{
		return;
	}
	else
	{
		Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	}
}


APlayerController* AMyProject4GameModeBase::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	if ((Cast<UMyGameInstance>(GetGameInstance())->_gameStarted))
	{
		return NULL;
	}
	else
	{
		return Super::Login(NewPlayer, InRemoteRole, Options, Portal, UniqueId, ErrorMessage);
	}
}

void AMyProject4GameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}



void AMyProject4GameModeBase::CountTime(float DeltaTime)
{

	if (_bBoss)
	{
		_bossTimer -= DeltaTime;
		if (_bossTimer < 0)
		{
			_bossTimer = 0;
			SpawnBoss();
			_bBoss = false;
		}
	}

	if (_bIsFinal)
	{

		_finalTimer -= DeltaTime;

		_gameState->_finalTimer = _finalTimer;
		if (_finalTimer < 0)
		{
			_finalTimer = 0;
			_bPaused = true;
			_bIsFinal = false;
			UE_LOG(LogTemp, Warning, TEXT("GAME over!!"));
			//game stop here

			_gameState->NotifyFin();
		}
	}


	if (!_bIsStarted)
	{
		_StartTimer -= DeltaTime;
		_gameState = GetGameState<AMyGameStateBase>();
		if (_gameState)
		{
			_gameState->_StartTimer = _StartTimer;
			if (_StartTimer < 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("%f"), _StartTimer);
				_gameState->LetPlayerMove();
				_bIsStarted = true;			
			}
		}
	}

}



void AMyProject4GameModeBase::SpawnBoss()
{
	if (_enemyAsset)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		AActor* enemyClass = GetWorld()->SpawnActor<AActor>(_enemyAsset, _AIRespawnPoint, FRotator(0.f), SpawnParams);
		_enemy = Cast<AMyEnemy>(enemyClass);

		if (_enemy)
		{
			_enemy->SpawnDefaultController();
			AAIController* AICon = Cast<AAIController>(_enemy->GetController());
			if (AICon)
			{
				_enemy->_AIController = AICon;
			}
		}
	}
}

