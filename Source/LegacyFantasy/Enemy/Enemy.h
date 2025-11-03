// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "PaperZDAnimInstance.h"
#include "Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyActionDelegate);

UCLASS()
class LEGACYFANTASY_API AEnemy : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	
	/* Find Character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	class USphereComponent* CharacterDetectorSphere;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	class ASelectedCharacter* FollowTarget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	float StopDistanceToTarget = 70.f;

	virtual bool ShouldMoveToTarget();

	/* Direction */
	virtual void UpdateDirection(float MDirection);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	float MoveDirection = -1.f;

	/* Movement */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	class UCharacterMovementComponent* EnemyMovement;
	
	/* Move */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	bool CanMove = true;

	/* Run */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	const UPaperZDAnimSequence* RunAnimSequence;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	bool CanRun = true;
	
	virtual void Run();
	virtual void OnRunOverrideAnimEnd(bool Completed);
	
	/* Attack */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	class UBoxComponent* AttackCollisionBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	const UPaperZDAnimSequence* AttackAnimSequence;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	float AttackCooldownInSeconds = 3.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	bool CanAttack = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	int DamageAttack = 20;
	
	virtual void Attack();
	virtual void OnAttackCooldownTimerTimeout();
	virtual void OnAttackOverrideAnimEnd(bool Completed);

public:
	/* Take damage */
	virtual void TakeDamage(int DamageAmount);

protected:
	/* HP */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	int HP = 100;

	virtual void SetHP(int NewHP);
	
public:
	virtual int GetHP();

protected:

	/* Status */
	bool IsAlive = true;
	
	/* Die */
	virtual void Die();

	/* Stun */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	float StunCooldownInSeconds = 1.5f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	bool IsStunned = false;

	virtual void Stun();
	virtual void OnStunTimerTimeout();
	
	/* Event, function callback */
	
};
