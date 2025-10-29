// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkEnemy.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LegacyFantasy/Character/SelectedCharacter.h"

AWalkEnemy::AWalkEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CharacterDetectorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CharacterDetectorSphere"));
	CharacterDetectorSphere->SetupAttachment(RootComponent);

	AttackCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionBox"));
	AttackCollisionBox->SetupAttachment(RootComponent);

	UpPlatformDetectorBox = CreateDefaultSubobject<UBoxComponent>(TEXT("UpPlatformDetectorBox"));
	UpPlatformDetectorBox->SetupAttachment(RootComponent);

	DownPlatformDetectorBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DownPlatformDetectorBox"));
	DownPlatformDetectorBox->SetupAttachment(RootComponent);
}

void AWalkEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemyMovement = GetCharacterMovement();
	
	CharacterDetectorSphere->OnComponentBeginOverlap.AddDynamic(this, &AWalkEnemy::DetectorOverlapBegin);
	CharacterDetectorSphere->OnComponentEndOverlap.AddDynamic(this, &AWalkEnemy::DetectorOverlapEnd);

	OnAttackOverrideEndDelegate.BindUObject(this, &AWalkEnemy::OnAttackOverrideAnimEnd);
	
	AttackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWalkEnemy::AttackBoxOverlapBegin);
	EnableAttackCollisionBox(false);

	OnRunOverrideEndDelegate.BindUObject(this, &AWalkEnemy::OnRunOverrideAnimEnd);

	UpPlatformDetectorBox->OnComponentBeginOverlap.AddDynamic(this, &AWalkEnemy::OverlapBegin);
	DownPlatformDetectorBox->OnComponentEndOverlap.AddDynamic(this, &AWalkEnemy::OverlapEnd);
}

void AWalkEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AWalkEnemy::EnemyAction()
{
	if (IsAlive)
	{
		if (FollowTarget && FollowTarget->GetStatusCharacter() && FMath::FloorToInt(FollowTarget->GetActorLocation().Z) == FMath::FloorToInt(GetActorLocation().Z))
		{
			MoveDirection = (FollowTarget->GetActorLocation().X - GetActorLocation().X) > 0.f ? 1.f : -1.f;
			UpdateDirection(MoveDirection);

			if (ShouldMoveToTarget())
			{
				FVector WorldDirection = FVector(1.f, 0.f, 0.f);
				AddMovementInput(WorldDirection, MoveDirection);
				EnemyMovement->MaxWalkSpeed = 300.f;
				Run();
			}
			else
			{
				Attack();
			}
		}
		else
		{
			if (CanWalk)
			{
				FVector WorldDirection = FVector(1.f, 0.f, 0.f);
				AddMovementInput(WorldDirection, MoveDirection);
				EnemyMovement->MaxWalkSpeed = 100.f;
				UpdateDirection(MoveDirection);
			}
		}
	}
	
	EnemyActionDelegate.Broadcast();
}

bool AWalkEnemy::ShouldMoveToTarget()
{
	if (FollowTarget)
	{
		float DistToTarget = abs(FollowTarget->GetActorLocation().X - GetActorLocation().X);
		return (DistToTarget > StopDistanceToTarget);
	}
	return false;
}

void AWalkEnemy::UpdateDirection(float MDirection)
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

void AWalkEnemy::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	MoveDirection = -MoveDirection;
	UpdateDirection(MoveDirection);
}

void AWalkEnemy::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int OtherBodyIndex)
{
	MoveDirection = -MoveDirection;
	UpdateDirection(MoveDirection);
}

void AWalkEnemy::Run()
{
	if (CanRun)
	{
		CanRun = false;
		
		GetAnimInstance()->PlayAnimationOverride(RunAnimSequence, FName("RunSlot"), 1.f,
												 0.f, OnRunOverrideEndDelegate);
	}
}

void AWalkEnemy::OnRunOverrideAnimEnd(bool Completed)
{
	CanRun = true;
}

void AWalkEnemy::DetectorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								  UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	ASelectedCharacter* Character = Cast<ASelectedCharacter>(OtherActor);

	if (Character && IsAlive && Character->GetStatusCharacter())
	{
		FollowTarget = Character;
	}
}

void AWalkEnemy::DetectorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	ASelectedCharacter* Character = Cast<ASelectedCharacter>(OtherActor);

	if (Character)
	{
		FollowTarget = NULL;
	}
}

void AWalkEnemy::AttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	ASelectedCharacter* Character = Cast<ASelectedCharacter>(OtherActor);
	
	if (Character && Character->GetStatusCharacter())
	{
		Character->TakeDamage(DamageAttack);
	}
}

void AWalkEnemy::Attack()
{
	if (CanAttack)
	{
		CanWalk = false;
		CanAttack = false;
	
		GetAnimInstance()->PlayAnimationOverride(AttackAnimSequence, FName("AttackSlot"), 1.f,
				0.f, OnAttackOverrideEndDelegate);
	
		// GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, TEXT("Enemy Attack!"));
		UE_LOG(LogTemp, Warning, TEXT("Enemy Attack!"));
	
		GetWorldTimerManager().SetTimer(AttackCooldownTimer, this, &AWalkEnemy::OnAttackCooldownTimerTimeout,
				1.f, false, AttackCooldownInSeconds);
	}
	
}

void AWalkEnemy::OnAttackCooldownTimerTimeout()
{
	CanAttack = true;
}

void AWalkEnemy::OnAttackOverrideAnimEnd(bool Completed)
{
	CanWalk = true;
}

void AWalkEnemy::EnableAttackCollisionBox(bool Enabled)
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

void AWalkEnemy::TakeDamage(int DamageAmount)
{
	if (!IsAlive) return;
	
	HP -= DamageAmount;

	if (HP <= 0)
	{
		Die();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Enemy HP: %d"), HP);
}

void AWalkEnemy::Die()
{
	SetHP(0);
	
	IsAlive = false;
	CanWalk = false;
	CanRun = false;
	CanAttack = false;

	FollowTarget = NULL;
		
	// Play the die animation
	GetAnimInstance()->JumpToNode(FName("JumpDie"), FName("Brown Boar State Machine"));
		
	EnableAttackCollisionBox(false);
}