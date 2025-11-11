// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PlayerAnim.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"  
#include "KismetAnimationLibrary.h"   

void UPlayerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UPlayerAnim::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (PlayerCharacter == nullptr) return;

	FVector Velocity = PlayerCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = PlayerCharacter->GetCharacterMovement()->IsFalling();
	bIsInputAccelerating = PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;

	FRotator ActorRotation = PlayerCharacter->GetActorRotation();
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, ActorRotation);
}
