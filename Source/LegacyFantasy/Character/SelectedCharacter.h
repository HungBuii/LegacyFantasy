// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDCharacter.h"
#include "SelectedCharacter.generated.h"


struct FInputActionValue;


UCLASS()
class LEGACYFANTASY_API ASelectedCharacter : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASelectedCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/* Realtime */
	// float RealTime = 0.f;
	
	/* Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	class UCameraComponent* Camera;

	/* Inputs */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	class UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
    class UInputAction* AttackAction;
	
	/* Move */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	bool CanMove = true;
	
	void Move(const FInputActionValue& Value);

	/* Jump */
	void JumpStarted(const FInputActionValue& Value);
	void JumpEnded(const FInputActionValue& Value);
	
	/* Attack */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	class UBoxComponent* AttackCollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	const UPaperZDAnimSequence* AttackAnimSequence;
	
	FZDOnAnimationOverrideEndSignature OnAttackOverrideEndDelegate;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	bool CanAttack = true;
	
	void Attack(const FInputActionValue& Value);

	void OnAttackOverrideAnimEnd(bool Completed);

	UFUNCTION(BlueprintCallable)
	void EnableAttackCollisionBox(bool Enabled);

	UFUNCTION()
	void AttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	float AttackCooldownInSeconds = 3.f;
	
	struct FTimerHandle AttackCooldownTimer;

	void OnAttackCooldownTimerTimeout();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	int DamageAttack = 25;
	
	/* Direction */
	void UpdateDirection(float MoveDirection);

public:
	/* Take damage */
	void TakeDamage(int DamageAmount);

private:
	/* HP */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	int HP = 100;

	void SetHP(int NewHP);

public:
	int GetHP();

private:
	/* Status */
	bool IsAlive = true;
	
public:
	bool GetStatusCharacter();	

private:
	/* Die */
	void Die();

	/* HUD */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	TSubclassOf<class UCharacterHUD> CharacterHUDClass;

	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess))
	class UCharacterHUD* CharacterHUDWidget;
};
