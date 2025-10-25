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

	UpPlatformDetectorBox = CreateDefaultSubobject<UBoxComponent>(TEXT("UpPlatformDetectorBox"));
	UpPlatformDetectorBox->SetupAttachment(RootComponent);

	DownPlatformDetectorBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DownPlatformDetectorBox"));
	DownPlatformDetectorBox->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	CharacterDetectorSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::DetectorOverlapBegin);
	CharacterDetectorSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::DetectorOverlapEnd);

	OnAttackOverrideEndDelegate.BindUObject(this, &AEnemy::OnAttackOverrideAnimEnd);
	
	AttackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AttackBoxOverlapBegin);
	EnableAttackCollisionBox(false);

	OnRunOverrideEndDelegate.BindUObject(this, &AEnemy::OnRunOverrideAnimEnd);

	UpPlatformDetectorBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OverlapBegin);
	DownPlatformDetectorBox->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OverlapEnd);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FollowTarget && FMath::FloorToInt(FollowTarget->GetActorLocation().Z) == FMath::FloorToInt(GetActorLocation().Z))
	{
		MoveDirection = (FollowTarget->GetActorLocation().X - GetActorLocation().X) > 0.f ? 1.f : -1.f;
		UpdateDirection(MoveDirection);

		if (ShouldMoveToTarget())
		{
			FVector WorldDirection = FVector(1.f, 0.f, 0.f);
			AddMovementInput(WorldDirection, MoveDirection);
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
			UpdateDirection(MoveDirection);
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

void AEnemy::UpdateDirection(float MDirection)
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

void AEnemy::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	MoveDirection = -MoveDirection;
	UpdateDirection(MoveDirection);
}

void AEnemy::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int OtherBodyIndex)
{
	MoveDirection = -MoveDirection;
	UpdateDirection(MoveDirection);
}

void AEnemy::Run()
{
	if (CanRun)
	{
		CanRun = false;
		
		GetAnimInstance()->PlayAnimationOverride(RunAnimSequence, FName("RunSlot"), 1.f,
		                                         0.f, OnRunOverrideEndDelegate);
	}
}

void AEnemy::OnRunOverrideAnimEnd(bool Completed)
{
	CanRun = true;
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

void AEnemy::AttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	ASelectedCharacter* Character = Cast<ASelectedCharacter>(OtherActor);
	
	if (Character)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, TEXT("Character is attacked!"));
	}
}

void AEnemy::Attack()
{
	if (CanAttack)
	{
		CanWalk = false;
		CanAttack = false;
	
		GetAnimInstance()->PlayAnimationOverride(AttackAnimSequence, FName("AttackSlot"), 1.f,
				0.f, OnAttackOverrideEndDelegate);
	
		// GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, TEXT("Enemy Attack!"));
		UE_LOG(LogTemp, Warning, TEXT("Enemy Attack!"));
	
		GetWorldTimerManager().SetTimer(AttackCooldownTimer, this, &AEnemy::OnAttackCooldownTimerTimeout,
				1.f, false, AttackCooldownInSeconds);
	}
	
}

void AEnemy::OnAttackCooldownTimerTimeout()
{
	CanAttack = true;
}

void AEnemy::OnAttackOverrideAnimEnd(bool Completed)
{
	CanWalk = true;
}

void AEnemy::EnableAttackCollisionBox(bool Enabled)
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
