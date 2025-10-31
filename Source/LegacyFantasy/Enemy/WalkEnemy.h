// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "WalkEnemy.generated.h"

/**
 * 
 */
UCLASS()
class LEGACYFANTASY_API AWalkEnemy : public AEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWalkEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	/* Check platform */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	class UBoxComponent* UpPlatformDetectorBox;

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	class UBoxComponent* DownPlatformDetectorBox;
	
	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex);

	/* Find Character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	class USphereComponent* CharacterDetectorSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	class ASelectedCharacter* FollowTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	float StopDistanceToTarget = 70.f;
	
	bool ShouldMoveToTarget();

	UFUNCTION()
	void DetectorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DetectorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex);

	/* Direction */
	void UpdateDirection(float MDirection);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	float MoveDirection = -1.f;
	
	/* Movement */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	class UCharacterMovementComponent* EnemyMovement;
	
	/* Move */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	bool CanWalk = true;

	/* Run */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	const UPaperZDAnimSequence* RunAnimSequence;
	
	FZDOnAnimationOverrideEndSignature OnRunOverrideEndDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	bool CanRun = true;
	
	void Run();
	void OnRunOverrideAnimEnd(bool Completed);
	
	/* Attack */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	class UBoxComponent* AttackCollisionBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	const UPaperZDAnimSequence* AttackAnimSequence;

	struct FTimerHandle AttackCooldownTimer;
	
	FZDOnAnimationOverrideEndSignature OnAttackOverrideEndDelegate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	float AttackCooldownInSeconds = 3.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	bool CanAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	int DamageAttack = 20;

	UFUNCTION()
	void AttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
	void EnableAttackCollisionBox(bool Enabled);
	
	void Attack();
	void OnAttackCooldownTimerTimeout();
	void OnAttackOverrideAnimEnd(bool Completed);

public:
	/* Take damage */
	virtual void TakeDamage(int DamageAmount) override;

	/* HP */
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	// int HP = 100;

protected:
	void SetHP(int NewHP) override;
	
public:
	int GetHP() override;

private:
	/* Status */
	bool IsAlive = true;
	
	/* Die */
	void Die();

	/* Stun */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	int StunCooldownInSeconds = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	bool IsStunned = false;

	struct FTimerHandle StunTimer;

	void Stun();
	void OnStunTimerTimeout();
	
	/* Event, function callback */
	UPROPERTY(BlueprintAssignable)
	FEnemyActionDelegate EnemyActionDelegate;

	UFUNCTION(BlueprintCallable)
	void EnemyAction();
};
