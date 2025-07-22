// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    if(Pawn == nullptr)
    {
        Pawn = TryGetPawnOwner();
        if(Pawn)
        {
            PlayerCharacter = Cast<APlayerCharacter>(Pawn);
        }
    }
}

void UMainAnimInstance::UpdateAnimationProperties()
{
    if (!Pawn)
    {
        Pawn = TryGetPawnOwner();
        PlayerCharacter = Pawn ? Cast<APlayerCharacter>(Pawn) : nullptr;
    }

    if (Pawn)
    {
        const FVector Velocity = Pawn->GetVelocity();
        const FVector Lateral = FVector(Velocity.X, Velocity.Y, 0.f);
        MovementSpeed = Lateral.Size();

        bIsAccelerating = PlayerCharacter && PlayerCharacter->GetCharacterMovement()
            ->GetCurrentAcceleration().Size() > 0.f;

        // New: set walk and run flags
        bIsWalking = MovementSpeed > WalkSpeedThreshold && MovementSpeed < RunSpeedThreshold;
        bIsRunning = MovementSpeed >= RunSpeedThreshold;
    }
}
