// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (PlayerCharacter)
	{
		PlayerCharacterMovement = PlayerCharacter->GetCharacterMovement();
	}
}

void UMainAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (PlayerCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerCharacterMovement->Velocity);
		bIsSprinting = GroundSpeed >= SprintSpeed;
		bIsFalling = PlayerCharacterMovement->IsFalling();
		bIsCrouching = PlayerCharacterMovement->IsCrouching();
	}
	else
	{
		GroundSpeed = 0.0f;
		bIsSprinting = false;
		bIsCrouching = false;
	}
}
