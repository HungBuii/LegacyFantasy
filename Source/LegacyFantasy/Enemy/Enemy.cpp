// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "LegacyFantasy/Character/SelectedCharacter.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CharacterDetectorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CharacterDetectorSphere"));
	CharacterDetectorSphere->SetupAttachment(RootComponent);

	AttackCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionBox"));
	AttackCollisionBox->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	CharacterDetectorSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::DetectorOverlapBegin);
	CharacterDetectorSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::DetectorOverlapEnd);

	OnAttackOverrideEndDelegate.BindUObject(this, &AEnemy::OnAttackOverrideAnimEnd);
	
	AttackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AttackBoxOverlapBegin);
	EnableAttackCollisionBox(false);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FollowTarget)
	{
		float MoveDirection = (FollowTarget->GetActorLocation().X - GetActorLocation().X) > 0.f ? 1.f : -1.f;
		UpdateDirection(MoveDirection);

		if (ShouldMoveToTarget())
		{
			FVector WorldDirection = FVector(1.f, 0.f, 0.f);
			AddMovementInput(WorldDirection, MoveDirection);
		}
		else
		{
			Attack();
		}
	}
}

void AEnemy::UpdateDirection(float MoveDirection)
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

bool AEnemy::ShouldMoveToTarget()
{
	if (FollowTarget)
	{
		float DistToTarget = abs(FollowTarget->GetActorLocation().X - GetActorLocation().X);
		return (DistToTarget > StopDistanceToTarget);
	}
	return false;
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

// void AEnemy::AttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
// {
// 	ASelectedCharacter* Character = Cast<ASelectedCharacter>(OtherActor);
// 	
// 	if (Character)
// 	{
// 		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, TEXT("Character is attacked!"));
// 	}
// }
//
// void AEnemy::Attack()
// {
// 	CanMove = false;
// 	CanAttack = false;
// 	
// 	GetAnimInstance()->PlayAnimationOverride(AttackAnimSequence, FName("AttackSlot"), 1.f,
// 			0.f, OnAttackOverrideEndDelegate);
//
// 	GetWorldTimerManager().SetTimer(AttackCooldownTimer, this, &AEnemy::OnAttackCooldownTimerTimeout,
// 			1.f, false, AttackCooldownInSeconds);
// }
//
// void AEnemy::OnAttackCooldownTimerTimeout()
// {
// 	CanAttack = true;
// }
//
// void AEnemy::OnAttackOverrideAnimEnd(bool Completed)
// {
// 	CanMove = true;
// }
//
// void AEnemy::EnableAttackCollisionBox(bool Enabled)
// {
// 	if (Enabled)
// 	{
// 		AttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
// 		AttackCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
// 	}
// 	else
// 	{
// 		AttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
// 		AttackCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
// 	}
// }
