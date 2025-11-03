// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorExit.h"

#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LegacyFantasy/Character/SelectedCharacter.h"
#include "LegacyFantasy/Enemy/Enemy.h"
#include "LegacyFantasy/GameInstance/LegacyFantasyGameInstance.h"

// Sets default values
ADoorExit::ADoorExit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	DoorFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("DoorFlipbook"));
	DoorFlipbook->SetupAttachment(RootComponent);
	DoorFlipbook->SetPlayRate(0.f);
	DoorFlipbook->SetLooping(false);
}

// Called when the game starts or when spawned
void ADoorExit::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ADoorExit::OverlapBegin);

	DoorFlipbook->SetPlaybackPosition(0.f, false);
}

// Called every frame
void ADoorExit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorExit::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	ASelectedCharacter* Character = Cast<ASelectedCharacter>(OtherActor);
	if (Character && Character->GetStatusCharacter())
	{
		if (IsActive)
		{
			Character->Deactivate();

			TArray<AActor*> Enemies;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), Enemies);
			for (AActor* EnemyActor : Enemies)
			{
				AEnemy* Enemy = Cast<AEnemy>(EnemyActor);
				if (Enemy && Enemy->GetStatus())
				{
					Enemy->Deactivate();
				}
			}
			
			IsActive = false;

			DoorFlipbook->SetPlayRate(1.f);
			DoorFlipbook->PlayFromStart();
			
			GetWorldTimerManager().SetTimer(WaitTimer, this, &ADoorExit::OnWaitTimerTimeout,
				1.f, false, WaitTimeInSeconds);
		}
	}
}

void ADoorExit::OnWaitTimerTimeout()
{
	ULegacyFantasyGameInstance* GameInstance = Cast<ULegacyFantasyGameInstance>(GetGameInstance());

	if (GameInstance)
	{
		GameInstance->ChangeLevel(LevelIndex);
	}
}

