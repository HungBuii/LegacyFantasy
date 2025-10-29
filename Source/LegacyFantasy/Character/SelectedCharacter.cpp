// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectedCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/BoxComponent.h"
#include "LegacyFantasy/Enemy/Enemy.h"


ASelectedCharacter::ASelectedCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	AttackCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionBox"));
	AttackCollisionBox->SetupAttachment(RootComponent);
}

void ASelectedCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* CharacterController = Cast<APlayerController>(Controller);
	if (CharacterController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
														(CharacterController->GetLocalPlayer());

		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	OnAttackOverrideEndDelegate.BindUObject(this, &ASelectedCharacter::OnAttackOverrideAnimEnd);

	AttackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASelectedCharacter::AttackBoxOverlapBegin);
	EnableAttackCollisionBox(false);
}

void ASelectedCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// UE_LOG(LogTemp, Warning, TEXT("Elapsed Time: %.2f seconds"), RealTime);
	// RealTime += DeltaTime;
}

void ASelectedCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASelectedCharacter::Move);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASelectedCharacter::JumpStarted);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASelectedCharacter::JumpEnded);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Canceled, this, &ASelectedCharacter::JumpEnded);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASelectedCharacter::Attack);
	}
}

void ASelectedCharacter::Move(const FInputActionValue& Value)
{
	float MoveActionValue = Value.Get<float>();

	if (CanMove && IsAlive)
	{
		FVector WorldDirection = FVector(1.f, 0, 0);
		AddMovementInput(WorldDirection, MoveActionValue);
		UpdateDirection(MoveActionValue);
	}
	
}

void ASelectedCharacter::UpdateDirection(float MoveDirection)
{
	FRotator CurrentRotation = Controller->GetControlRotation();

	if (MoveDirection < 0.f)
	{
		if (CurrentRotation.Yaw != 180.f)
		{
			Controller->SetControlRotation(FRotator(CurrentRotation.Pitch, 180.f, CurrentRotation.Roll));
		}
	}
	else if (MoveDirection > 0.f)
	{
		if (CurrentRotation.Yaw != 0.f)
		{
			Controller->SetControlRotation(FRotator(CurrentRotation.Pitch, 0.f, CurrentRotation.Roll));
		}
	}
}

void ASelectedCharacter::JumpStarted(const FInputActionValue& Value)
{
	Jump();
}

void ASelectedCharacter::JumpEnded(const FInputActionValue& Value)
{
	StopJumping();
}

void ASelectedCharacter::Attack(const FInputActionValue& Value)
{
	if (CanAttack && IsAlive)
	{
		CanAttack = false;
		CanMove = false;
		
		GetAnimInstance()->PlayAnimationOverride(AttackAnimSequence, FName("AttackSlot"), 1.f,
				0.f, OnAttackOverrideEndDelegate);

		GetWorldTimerManager().SetTimer(AttackCooldownTimer, this,
			&ASelectedCharacter::OnAttackCooldownTimerTimeout, 1.f, false, AttackCooldownInSeconds);
	}
}

void ASelectedCharacter::OnAttackCooldownTimerTimeout()
{
	CanAttack = true;
}

void ASelectedCharacter::OnAttackOverrideAnimEnd(bool Completed)
{
	CanMove = true;
}

void ASelectedCharacter::EnableAttackCollisionBox(bool Enabled)
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

void ASelectedCharacter::AttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);

	if (Enemy)
	{
		Enemy->TakeDamage(DamageAttack);
	}
}

void ASelectedCharacter::TakeDamage(int DamageAmount)
{
	if (!IsAlive) return;

	HP -= DamageAmount;
	if (HP <= 0)
	{
		Die();
	}

	UE_LOG(LogTemp, Warning, TEXT("Character HP: %d"), HP);
}

void ASelectedCharacter::SetHP(int NewHP)
{
	HP = NewHP;
}

int ASelectedCharacter::GetHP()
{
	return HP;
}

void ASelectedCharacter::Die()
{
	SetHP(0);
	
	IsAlive = false;
	CanMove = false;
	CanAttack = false;

	// Play the die animation
	GetAnimInstance()->JumpToNode(FName("JumpDie"), FName("Character State Machine"));

	EnableAttackCollisionBox(false);
}

bool ASelectedCharacter::GetStatusCharacter()
{
	return IsAlive;
}

