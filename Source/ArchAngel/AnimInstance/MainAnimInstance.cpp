// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchAngel/AnimInstance/MainAnimInstance.h"
#include "ArchAngel/Character/TopDownCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	// Cache the owning character and its movement component
	OwnerCharacter = Cast<ATopDownCharacter>(TryGetPawnOwner());
    if (OwnerCharacter)
    {
        MovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void UMainAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (OwnerCharacter && MovementComponent)
    {
        // Get horizontal velocity, immediately ignoring Z component
        const FVector Velocity = MovementComponent->Velocity;
        // Use GetSafeNormal() to normalize the vector and handle KINDA_SMALL_NUMBER internally
        const FVector MoveDir = FVector(Velocity.X, Velocity.Y, 0.f).GetSafeNormal();

        // Speed is the magnitude of the lateral velocity
        Speed = Velocity.Size2D();

        // If the character is moving (MoveDir is not zero vector after GetSafeNormal())
        if (!MoveDir.IsNearlyZero())
        {
            // Actor forward vector (XY plane)
            // This is the direction the character is aiming/facing
            const FVector Forward = OwnerCharacter->GetActorForwardVector();
            const FVector Forward2D = FVector(Forward.X, Forward.Y, 0.f).GetSafeNormal();

            // Calculate the signed angle (Direction) between the Aiming direction and the Movement direction

            // angle = atan2(cross.z, dot). range = [-PI, PI]
            const float Dot = FVector::DotProduct(Forward2D, MoveDir);
            const float CrossZ = FVector::CrossProduct(Forward2D, MoveDir).Z;
            const float AngleRad = FMath::Atan2(CrossZ, Dot);

            // Convert to degrees in [-180, 180]
            Direction = FMath::RadiansToDegrees(AngleRad);
        }
        else
        {
            // If not moving, Direction can safely be set to 0.0f
            Direction = 0.0f;
        }
    }
}
