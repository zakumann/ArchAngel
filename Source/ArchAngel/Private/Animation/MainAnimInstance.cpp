// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MainAnimInstance.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PoseSearch/PoseSearchLibrary.h"
#include "PoseSearch/PoseSearchDatabase.h"

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

	if (PlayerCharacter)
	{
		FVector Velocity = PlayerCharacter->GetVelocity();
		Velocity.Z = 0.f;
		Speed = Velocity.Size();

		// Calculate movement direction relative to character rotation
		FRotator ActorRotation = PlayerCharacter->GetActorRotation();
		Direction = CalculateDirection(Velocity, ActorRotation);
	}
}