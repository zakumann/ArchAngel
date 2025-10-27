// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MainAnimInstance.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/Controller.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UMainAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (!PlayerCharacter)
	{
		PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	}
	if (!PlayerCharacter) return;

	if (PlayerCharacter)
	{
		FVector Velocity = PlayerCharacter->GetVelocity();
		Velocity.Z = 0; // Ignore vertical velocity for speed

		Speed = Velocity.Size();

		// Calculate movement direction relative to actor rotation
		Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, PlayerCharacter->GetActorRotation());

		// Calculate movement direction relative to character rotation
		FRotator ActorRotation = PlayerCharacter->GetActorRotation();

		// Aiming
		FRotator ControlRot = PlayerCharacter->GetControlRotation();
		FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(ControlRot, ActorRotation);

		AimYaw = DeltaRot.Yaw;
		AimPitch = DeltaRot.Pitch;
	}
}