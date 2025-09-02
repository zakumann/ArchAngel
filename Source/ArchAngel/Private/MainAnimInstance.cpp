// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>

void UMainAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (!PlayerCharacter)
	{
		PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
		if (PlayerCharacter)
		{
			PlayerCharacterMovement = PlayerCharacter->GetCharacterMovement();
		}
	}
}

void UMainAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (!PlayerCharacter)
	{
		PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	}

    if (PlayerCharacterMovement && PlayerCharacter)
    {
        // 🔹 Speed
        GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerCharacterMovement->Velocity);

        // 🔹 State checks
        bIsWalking = PlayerCharacter->bIsWalking;
        bIsFalling = PlayerCharacterMovement->IsFalling();
        bIsCrouching = PlayerCharacterMovement->IsCrouching();

        // 🔹 Movement Direction (for BlendSpace)
        MovementDirection = CalculateDirection(PlayerCharacterMovement->Velocity, PlayerCharacter->GetActorRotation());

        // 🔹 Convert velocity into local forward/right movement
        FVector Velocity = PlayerCharacterMovement->Velocity;
        FVector Forward = PlayerCharacter->GetActorForwardVector();
        FVector Right = PlayerCharacter->GetActorRightVector();

        // Normalize only on XY plane
        FVector NormalizedVel = Velocity.GetSafeNormal2D();

        ForwardValue = FVector::DotProduct(NormalizedVel, Forward);
        RightValue = FVector::DotProduct(NormalizedVel, Right);

        // 🔹 Aiming state (get from character, or keep as is for now)
        bIsAiming = PlayerCharacter->bIsAiming; // <-- make sure your PlayerCharacter has this
    }
    else
    {
        GroundSpeed = 0.0f;
        bIsWalking = false;
        bIsFalling = false;
        bIsCrouching = false;
        MovementDirection = 0.0f;
        ForwardValue = 0.0f;
        RightValue = 0.0f;
        bIsAiming = false;
    }
}
