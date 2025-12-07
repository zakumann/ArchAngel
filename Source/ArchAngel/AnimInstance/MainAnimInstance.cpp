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
        // Get horizontal velocity
        const FVector Velocity = MovementComponent->Velocity;
        const FVector LateralVelocity = FVector(Velocity.X, Velocity.Y, 0.f);

        // Speed for blendspaces
        Speed = LateralVelocity.Size();

        // If not moving, direction is 0 (or keep last value if you prefer)
        const float SpeedSq = LateralVelocity.SizeSquared();
        if (SpeedSq <= KINDA_SMALL_NUMBER)
        {
            Direction = 0.0f;
        }
        else
        {
            // Normalize movement direction (XY plane)
            const FVector MoveDir = LateralVelocity / FMath::Sqrt(SpeedSq);

            // Actor forward vector (XY plane)
            const FVector Forward = OwnerCharacter->GetActorForwardVector();
            const FVector Forward2D = FVector(Forward.X, Forward.Y, 0.f).GetSafeNormal();

            // Signed angle between Forward2D and MoveDir:
            // angle = atan2(cross.z, dot). range = [-PI, PI]
            const float Dot = FVector::DotProduct(Forward2D, MoveDir);
            const float CrossZ = FVector::CrossProduct(Forward2D, MoveDir).Z;
            const float AngleRad = FMath::Atan2(CrossZ, Dot);

            // Convert to degrees in [-180, 180]
            Direction = FMath::RadiansToDegrees(AngleRad);
        }
    }
}
