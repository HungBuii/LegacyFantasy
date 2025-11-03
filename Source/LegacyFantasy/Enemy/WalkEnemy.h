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
	virtual bool ShouldMoveToTarget() override;

	UFUNCTION()
	void DetectorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DetectorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex);

	/* Direction */
	virtual void UpdateDirection(float MDirection) override;
	
	/* Movement */
	
	/* Move */

	/* Run */
	FZDOnAnimationOverrideEndSignature OnRunOverrideEndDelegate;
	
	virtual void Run() override;
	virtual void OnRunOverrideAnimEnd(bool Completed) override;
	
	/* Attack */
	struct FTimerHandle AttackCooldownTimer;
	
	FZDOnAnimationOverrideEndSignature OnAttackOverrideEndDelegate;

	UFUNCTION()
	void AttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
	void EnableAttackCollisionBox(bool Enabled);
	
	virtual void Attack() override;
	virtual void OnAttackCooldownTimerTimeout() override;
	virtual void OnAttackOverrideAnimEnd(bool Completed) override;

public:
	/* Take damage */
	virtual void TakeDamage(int DamageAmount) override;

	/* HP */

protected:
	virtual void SetHP(int NewHP) override;
	
public:
	virtual int GetHP() override;
	
	/* Status */
	virtual bool GetStatus() override;
	
	/* Disable Action */
	virtual void Deactivate() override;

private:
	/* Die */
	virtual void Die() override;

	/* Stun */
	struct FTimerHandle StunTimer;

	virtual void Stun() override;
	virtual void OnStunTimerTimeout() override;
	
	/* Event, function callback */
	UPROPERTY(BlueprintAssignable)
	FEnemyActionDelegate EnemyActionDelegate;

	UFUNCTION(BlueprintCallable)
	void EnemyAction();
};
