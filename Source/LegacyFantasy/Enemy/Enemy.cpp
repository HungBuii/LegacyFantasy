// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "LegacyFantasy/Character/SelectedCharacter.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

bool AEnemy::ShouldMoveToTarget()
{
	return false;
}

void AEnemy::UpdateDirection(float MDirection)
{
	
}

void AEnemy::Run()
{
	
}

void AEnemy::OnRunOverrideAnimEnd(bool Completed)
{
	
}

void AEnemy::Attack()
{
	
}

void AEnemy::OnAttackCooldownTimerTimeout()
{
	
}

void AEnemy::OnAttackOverrideAnimEnd(bool Completed)
{
	
}

void AEnemy::TakeDamage(int DamageAmount)
{
	
}

void AEnemy::SetHP(int NewHP)
{
	HP = NewHP;
}

int AEnemy::GetHP()
{
	return HP;
}

bool AEnemy::GetStatus()
{
	return IsAlive;
}

void AEnemy::Deactivate()
{
	if (IsActiveAction)
	{
		IsActiveAction = false;
		CanAttack = false;
		CanMove = false;
		FollowTarget = NULL;
		
		GetCharacterMovement()->StopMovementImmediately();
	}
}

void AEnemy::Die()
{
	
}

void AEnemy::Stun()
{
}

void AEnemy::OnStunTimerTimeout()
{
}
