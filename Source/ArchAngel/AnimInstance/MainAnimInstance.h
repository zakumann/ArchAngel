// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

class ATopDownCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class ARCHANGEL_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	// Called every frame, equivalent to Event Blueprint Update Animation
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    // Variables to expose to the Animation Blueprint
    UPROPERTY(BlueprintReadOnly, Category = Movement)
    float Speed;

    UPROPERTY(BlueprintReadOnly, Category = Movement)
    float Direction;

private:
    // Cached references
    UPROPERTY()
    ATopDownCharacter* OwnerCharacter;

    UPROPERTY()
    UCharacterMovementComponent* MovementComponent;
};
