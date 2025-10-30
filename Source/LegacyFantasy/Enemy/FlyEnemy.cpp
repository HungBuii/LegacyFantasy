// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyEnemy.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LegacyFantasy/Character/SelectedCharacter.h"

AFlyEnemy::AFlyEnemy()
{
	CharacterDetectorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CharacterDetectorSphere"));
	CharacterDetectorSphere->SetupAttachment(RootComponent);
	
	PlatformDetectorBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PlatformDetectorBox"));
	PlatformDetectorBox->SetupAttachment(RootComponent);
}

void AFlyEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemyMovement = GetCharacterMovement();
	
	CharacterDetectorSphere->OnComponentBeginOverlap.AddDynamic(this, &AFlyEnemy::DetectorOverlapBegin);
	CharacterDetectorSphere->OnComponentEndOverlap.AddDynamic(this, &AFlyEnemy::DetectorOverlapEnd);
	
	PlatformDetectorBox->OnComponentBeginOverlap.AddDynamic(this, &AFlyEnemy::PlatformDetectorOverlapBegin);
}

void AFlyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FollowTarget)
	{
		MoveDirection = (FollowTarget->GetActorLocation().X - GetActorLocation().X) > 0.f ? 1.f : -1.f;
		UpdateDirection(MoveDirection);

		FVector DirectionToCharacter  = FollowTarget->GetActorLocation() - GetActorLocation();
		if (ShouldMoveToTarget(DirectionToCharacter))
		{
			DirectionToCharacter.Normalize();
			FVector NewLocation = GetActorLocation() + (DirectionToCharacter * MoveSpeed * DeltaTime);
			SetActorLocation(NewLocation);
		}
		else
		{
			// Attack
			UE_LOG(LogTemp, Warning, TEXT("Enemy Attack"));
		}
	}
	else
	{
		// FVector WorldDirection = FVector(1.f, 0.f, 0.f);
		// AddMovementInput(WorldDirection, MoveDirection);
		// EnemyMovement->MaxFlySpeed = MoveSpeed;
		// UpdateDirection(MoveDirection);

		UpdateDirection(MoveDirection);
		FVector NewLocation = GetActorLocation();
		NewLocation.X = GetActorLocation().X + (MoveDirection * MoveSpeed * DeltaTime);
		SetActorLocation(NewLocation);
	}
	
}

void AFlyEnemy::PlatformDetectorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	MoveDirection = -MoveDirection;
	UpdateDirection(MoveDirection);
}

bool AFlyEnemy::ShouldMoveToTarget(const FVector& DirectionToCharacter)
{
	if (FollowTarget)
	{
		float DistanceToCharacter = abs(DirectionToCharacter.Length());
		return (DistanceToCharacter > StopDistanceToTarget);
	}
	return false;
}

void AFlyEnemy::UpdateDirection(float MDirection)
{
	FRotator CurrentRotation = GetActorRotation();

	if (MDirection < 0.f)
	{
		if (CurrentRotation.Yaw != 0.f)
		{
			SetActorRotation(FRotator(CurrentRotation.Pitch, 0.f, CurrentRotation.Roll));
		}
	}
	else if (MDirection > 0.f)
	{
		if (CurrentRotation.Yaw != 180.f)
		{
			SetActorRotation(FRotator(CurrentRotation.Pitch, 180.f, CurrentRotation.Roll));
		}
	}
}

void AFlyEnemy::DetectorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	ASelectedCharacter* Character = Cast<ASelectedCharacter>(OtherActor);

	if (Character && Character->GetStatusCharacter())
	{
		FollowTarget = Character;
	}
}

void AFlyEnemy::DetectorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	ASelectedCharacter* Character = Cast<ASelectedCharacter>(OtherActor);

	if (Character)
	{
		FollowTarget = NULL;
	}
}
