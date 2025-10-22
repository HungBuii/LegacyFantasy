// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Components/SphereComponent.h"
#include "LegacyFantasy/Character/SelectedCharacter.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CharacterDetectorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDetectorSphere"));
	CharacterDetectorSphere->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	CharacterDetectorSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::DetectorOverlapBegin);
	CharacterDetectorSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::DetectorOverlapEnd);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::DetectorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	ASelectedCharacter* Character = Cast<ASelectedCharacter>(OtherActor);

	if (Character)
	{
		FollowTarget = Character;
	}
}

void AEnemy::DetectorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	ASelectedCharacter* Character = Cast<ASelectedCharacter>(OtherActor);

	if (Character)
	{
		FollowTarget = NULL;
	}
}
