// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"
#include "Components/BoxComponent.h"
#include "MyCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMyWeapon::AMyWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	_skeletalMesh->SetupAttachment(GetRootComponent());

	_combatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	_combatCollision->SetupAttachment(GetRootComponent());

	_combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	_combatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	_combatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	_combatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	_EweaponState = EWeaponState::EWS_Pickup;

}

// Called when the game starts or when spawned
void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();
	_combatCollision->OnComponentBeginOverlap.AddDynamic(this, &AMyWeapon::CombatOnOverlapBegin);

}

// Called every frame
void AMyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyWeapon::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//in case of Fist
	if (OtherActor && OtherActor!=GetInstigator())
	{
		LaunchServer(OtherActor);
	}
}

void AMyWeapon::LaunchServer_Implementation(AActor* OtherActor)
{
	APawn* pawn = Cast<APawn>(OtherActor);
	FVector dir{ pawn->GetActorLocation() - GetActorLocation()};
	dir.Normalize();
	pawn->LaunchPawn(dir * 700, false, true);
}



//void AMyWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
//	//if ((_EweaponState == EWeaponState::EWS_Pickup) && OtherActor)
//	//{
//	//	AMyCharacter* character = Cast<AMyCharacter>(OtherActor);
//
//	//	if (character)
//	//	{
//	//		character->SetOverlappingItem(this);
//	//	}
//	//}
//	//������ ŉ���
//}
//
//
//void AMyWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
//	//if (OtherActor)
//	//{
//	//	AMyCharacter* character = Cast<AMyCharacter>(OtherActor);
//
//	//	if (character)
//	//	{
//	//		character->SetOverlappingItem(nullptr);
//	//	}
//	//}
//	//������ ŉ���
//}


void AMyWeapon::Equip(AMyCharacter* character)
{
	//if (character)
	//{
	//	_skeletalMesh = character->GetMesh();
	//	_skeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	//	_skeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	//	_skeletalMesh->SetSimulatePhysics(false);

	//	const USkeletalMeshSocket* AttackSocket = _skeletalMesh->GetSocketByName("AttackSocket");
	//	if (AttackSocket)
	//	{
	//		AttackSocket->AttachActor(this, character->GetMesh());
	//		_bRotate = false;

	//		character->SetEquippedWeapon(this);
	//		character->SetOverlappingItem(nullptr);
	//	}
	//}
	// ������ ŉ���
}