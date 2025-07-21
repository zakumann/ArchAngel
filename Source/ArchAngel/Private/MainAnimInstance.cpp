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
    if(Pawn == nullptr)
    {
        Pawn = TryGetPawnOwner();
    }

    if(Pawn)
    {
        FVector Speed = Pawn->GetVelocity();
        FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
        MovementSpeed = LateralSpeed.Size();

        if(PlayerCharacter == nullptr)
        {
            PlayerCharacter = Cast<APlayerCharacter>(Pawn);
        }

        if(PlayerCharacter)
        {
            if(PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0)
            {
                bIsAccelerating = true;
            }
            else
            {
                bIsAccelerating = false;
            }
        }
    }
}
