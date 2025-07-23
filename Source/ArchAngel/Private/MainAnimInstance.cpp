// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    Pawn = TryGetPawnOwner();
    PlayerCharacter = Pawn ? Cast<APlayerCharacter>(Pawn) : nullptr;
}

void UMainAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!Pawn)
    {
        Pawn = TryGetPawnOwner();
        PlayerCharacter = Pawn ? Cast<APlayerCharacter>(Pawn) : nullptr;
    }

    UpdateAnimationProperties();
}

void UMainAnimInstance::UpdateAnimationProperties()
{
    if (!Pawn) return;

    const FVector Velocity = Pawn->GetVelocity();
    MovementSpeed = FVector(Velocity.X, Velocity.Y, 0.f).Size();

    if (PlayerCharacter)
    {
        const FVector Accel = PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration();
        bIsAccelerating = Accel.Size() > 0.f;

        bIsWalking = MovementSpeed > WalkSpeedThreshold && MovementSpeed < RunSpeedThreshold;
        bIsRunning = MovementSpeed >= RunSpeedThreshold;
        bIsSprinting = PlayerCharacter->bIsSprinting;

        bHasWeapon = (PlayerCharacter->CurrentWeapon != nullptr);
    }
}
