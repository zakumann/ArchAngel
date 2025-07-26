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

    MovementSpeed = FVector(Pawn->GetVelocity().X, Pawn->GetVelocity().Y, 0.f).Size();

    if (APlayerCharacter* PC = Cast<APlayerCharacter>(Pawn))
    {
        bIsCrouching = PC->bIsCrouching;
        bIsSprinting = PC->bIsSprinting;
        bHasWeapon = (PC->CurrentWeapon != nullptr);
        bIsWalking = MovementSpeed > WalkSpeedThreshold && MovementSpeed < RunSpeedThreshold;
        bIsRunning = MovementSpeed >= RunSpeedThreshold;
        bIsAccelerating = PC->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;
    }
}
