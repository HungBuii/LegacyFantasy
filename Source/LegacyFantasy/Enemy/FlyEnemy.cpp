// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyEnemy.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LegacyFantasy/Character/SelectedCharacter.h"

AFlyEnemy::AFlyEnemy()
{
	CharacterDetectorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CharacterDetectorSphere"));
	CharacterDetectorSphere->SetupAttachment(RootComponent);
	
	PlatformDetectorBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PlatformDetectorBox"));
	PlatformDetectorBox->SetupAttachment(RootComponent);

	AttackCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionBox"));
	AttackCollisionBox->SetupAttachment(RootComponent);
}

void AFlyEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemyMovement = GetCharacterMovement();
	
	CharacterDetectorSphere->OnComponentBeginOverlap.AddDynamic(this, &AFlyEnemy::DetectorOverlapBegin);
	CharacterDetectorSphere->OnComponentEndOverlap.AddDynamic(this, &AFlyEnemy::DetectorOverlapEnd);
	
	PlatformDetectorBox->OnComponentBeginOverlap.AddDynamic(this, &AFlyEnemy::PlatformDetectorOverlapBegin);

	OnAttackOverrideEndDelegate.BindUObject(this, &AFlyEnemy::OnAttackOverrideAnimEnd);
	AttackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AFlyEnemy::AttackBoxOverlapBegin);
	EnableAttackCollisionBox(false);
}

void AFlyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFlyEnemy::EnemyAction()
{
	if (IsAlive)
	{
		if (FollowTarget && FollowTarget->GetStatusCharacter())
		{
			MoveDirection = (FollowTarget->GetActorLocation().X - GetActorLocation().X) > 0.f ? 1.f : -1.f;
			UpdateDirection(MoveDirection);
			
			if (ShouldMoveToTarget())
			{
				FVector DirectionToCharacter  = FollowTarget->GetActorLocation() - GetActorLocation();
				DirectionToCharacter.Normalize();
				FVector NewLocation = GetActorLocation() + (DirectionToCharacter * MoveSpeed * UGameplayStatics::GetWorldDeltaSeconds(this));
				SetActorLocation(NewLocation);
			}
			else
			{
				// Attack
				Attack();
			}
		}
		else
		{
			if (CanMove)
			{
				// FVector WorldDirection = FVector(1.f, 0.f, 0.f);
				// AddMovementInput(WorldDirection, MoveDirection);
				// EnemyMovement->MaxFlySpeed = MoveSpeed;
				// UpdateDirection(MoveDirection);

				UpdateDirection(MoveDirection);
				FVector NewLocation = GetActorLocation();
				NewLocation.X = GetActorLocation().X + (MoveDirection * MoveSpeed * UGameplayStatics::GetWorldDeltaSeconds(this));
				SetActorLocation(NewLocation);
			}
		}
	}
}

void AFlyEnemy::PlatformDetectorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	MoveDirection = -MoveDirection;
	UpdateDirection(MoveDirection);
}

bool AFlyEnemy::ShouldMoveToTarget()
{
	if (FollowTarget)
	{
		FVector DirectionToCharacter = FollowTarget->GetActorLocation() - GetActorLocation();
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

void AFlyEnemy::AttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	ASelectedCharacter* Character = Cast<ASelectedCharacter>(OtherActor);
	
	if (Character && Character->GetStatusCharacter())
	{
		Character->TakeDamage(DamageAttack);
	}
}

void AFlyEnemy::Attack()
{
	if (CanAttack && !IsStunned)
	{
		CanMove = false;
		CanAttack = false;
	
		GetAnimInstance()->PlayAnimationOverride(AttackAnimSequence, FName("AttackSlot"), 1.f,
				0.f, OnAttackOverrideEndDelegate);
	
		UE_LOG(LogTemp, Warning, TEXT("Bee Attack!"));
	
		GetWorldTimerManager().SetTimer(AttackCooldownTimer, this, &AFlyEnemy::OnAttackCooldownTimerTimeout,
				1.f, false, AttackCooldownInSeconds);
	}
}

void AFlyEnemy::OnAttackCooldownTimerTimeout()
{
	CanAttack = true;
}

void AFlyEnemy::OnAttackOverrideAnimEnd(bool Completed)
{
	CanMove = true;
}

void AFlyEnemy::EnableAttackCollisionBox(bool Enabled)
{
	if (Enabled)
	{
		AttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AttackCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
	else
	{
		AttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttackCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}
}

void AFlyEnemy::TakeDamage(int DamageAmount)
{
	if (!IsAlive) return;

	Stun();
	UE_LOG(LogTemp, Warning, TEXT("Bee Stun"));
	HP -= DamageAmount;

	if (HP <= 0)
	{
		Die();
	}
	else
	{
		GetAnimInstance()->JumpToNode(FName("JumpTakeHit"), FName("Bee State Machine"));
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Bee HP: %d"), HP);
}

void AFlyEnemy::SetHP(int NewHP)
{
	Super::SetHP(NewHP);
}

int AFlyEnemy::GetHP()
{
	return Super::GetHP();
}

bool AFlyEnemy::GetStatus()
{
	return Super::GetStatus();
}

void AFlyEnemy::Deactivate()
{
	Super::Deactivate();
}

void AFlyEnemy::Die()
{
	SetHP(0);
	
	IsAlive = false;
	CanMove = false;
	CanAttack = false;

	FollowTarget = NULL;
		
	EnableAttackCollisionBox(false);
	UE_LOG(LogTemp, Warning, TEXT("Bee Died"));
}

void AFlyEnemy::Stun()
{
	IsStunned = true;

	bool IsTimerAlreadyActive = GetWorldTimerManager().IsTimerActive(StunTimer);
	if (IsTimerAlreadyActive) GetWorldTimerManager().ClearTimer(StunTimer);
	
	GetWorldTimerManager().SetTimer(StunTimer, this, &AFlyEnemy::OnStunTimerTimeout,
		1.f, false, StunCooldownInSeconds);

	GetAnimInstance()->StopAllAnimationOverrides();

	EnableAttackCollisionBox(false);
}

void AFlyEnemy::OnStunTimerTimeout()
{
	IsStunned = false;
}
