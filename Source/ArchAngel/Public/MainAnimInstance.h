// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARCHANGEL_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	class APlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly)
	class UCharacterMovementComponent* PlayerCharacterMovement;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float GroundSpeed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsSprinting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsFalling = false;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsCrouching = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed = 600.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float MovementDirection = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float ForwardValue = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement") 
	float RightValue = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsAiming = false;
};
