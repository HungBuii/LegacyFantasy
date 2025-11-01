// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "FlyEnemy.generated.h"

/**
 * 
 */
UCLASS()
class LEGACYFANTASY_API AFlyEnemy : public AEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFlyEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	/* Check platform */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	class UBoxComponent* PlatformDetectorBox;

	UFUNCTION()
	void PlatformDetectorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	/* Find Character */
	bool ShouldMoveToTarget() override;

	UFUNCTION()
	void DetectorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DetectorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex);
	
	/* Direction */
	void UpdateDirection(float MDirection);

	/* Movement */
	
	/* Move */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	float MoveSpeed = 100.f;
	
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

protected:
	virtual void SetHP(int NewHP) override;
	
public:
	virtual int GetHP() override;

private:
	/* Status */

	/* Die */
	virtual void Die() override;

	/* Stun */
	struct FTimerHandle StunTimer;

	virtual void Stun() override;
	virtual void OnStunTimerTimeout() override;
};
