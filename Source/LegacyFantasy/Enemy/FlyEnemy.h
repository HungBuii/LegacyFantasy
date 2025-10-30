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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	class USphereComponent* CharacterDetectorSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	class ASelectedCharacter* FollowTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	float StopDistanceToTarget = 70.f;
	
	bool ShouldMoveToTarget(const FVector& DirectionToCharacter);

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	float MoveSpeed = 100.f;

	
};
