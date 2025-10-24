// Fill out your copyright notice in the Description page of Project Settings.


#include "OriginEnemy.h"

#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "LegacyFantasy/Character/SelectedCharacter.h"

// Sets default values
AOriginEnemy::AOriginEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	CharacterDetectorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CharacterDetectorSphere"));
	CharacterDetectorSphere->SetupAttachment(RootComponent);

	EnemyFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("EnemyFlipbook"));
	EnemyFlipbook->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AOriginEnemy::BeginPlay()
{
	Super::BeginPlay();

	CharacterDetectorSphere->OnComponentBeginOverlap.AddDynamic(this, &AOriginEnemy::DetectorOverlapBegin);
	CharacterDetectorSphere->OnComponentEndOverlap.AddDynamic(this, &AOriginEnemy::DetectorOverlapEnd);

	
}

// Called every frame
void AOriginEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FollowTarget)
	{
		float MoveDirection = (FollowTarget->GetActorLocation().X - GetActorLocation().X) > 0.f ? 1.f : -1.f;
		UpdateDirection(MoveDirection);
	
		if (ShouldMoveToTarget())
		{
			// Move towards the character
			FVector CurrentLocation = GetActorLocation();
			FVector CharacterLocation = FollowTarget->GetActorLocation();
			FVector DirectionToCharacter = CharacterLocation - CurrentLocation;
			
			DirectionToCharacter.Normalize();
			FVector NewLocation = CurrentLocation + (DirectionToCharacter * MovementSpeed * DeltaTime);
			SetActorLocation(NewLocation);
			
		}
		else
		{
			// Attack();
		}
	}
}

void AOriginEnemy::UpdateDirection(float MoveDirection)
{
	FRotator CurrentRotation = GetActorRotation();

	if (MoveDirection < 0.f)
	{
		if (CurrentRotation.Yaw != 0.f)
		{
			SetActorRotation(FRotator(CurrentRotation.Pitch, 0.f, CurrentRotation.Roll));
		}
	}
	else if (MoveDirection > 0.f)
	{
		if (CurrentRotation.Yaw != 180.f)
		{
			SetActorRotation(FRotator(CurrentRotation.Pitch, 180.f, CurrentRotation.Roll));
		}
	}
}

bool AOriginEnemy::ShouldMoveToTarget()
{
	if (FollowTarget)
	{
		float DistToTarget = abs(FollowTarget->GetActorLocation().X - GetActorLocation().X);
		return (DistToTarget > StopDistanceToTarget);
	}
	return false;
}

void AOriginEnemy::DetectorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	ASelectedCharacter* Character = Cast<ASelectedCharacter>(OtherActor);

	if (Character)
	{
		FollowTarget = Character;
	}
}

void AOriginEnemy::DetectorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	ASelectedCharacter* Character = Cast<ASelectedCharacter>(OtherActor);

	if (Character)
	{
		FollowTarget = NULL;
	}
}
