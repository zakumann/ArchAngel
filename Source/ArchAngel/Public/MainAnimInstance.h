// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EDodgeDirection : uint8
{
	Forward,
	Backward,
	Left,
	Right
};

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

	// === Dodge System ===
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dodge")
	bool bIsDodging = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dodge")
	FVector DodgeDirection = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dodge")
	EDodgeDirection DodgeDirEnum = EDodgeDirection::Forward;

protected:
	UPROPERTY(BlueprintReadOnly)
	class APlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly)
	class UCharacterMovementComponent* PlayerCharacterMovement;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float GroundSpeed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsWalking = false;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsFalling = false;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsCrouching = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float JogSpeed = 600.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float MovementDirection = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float ForwardValue = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement") 
	float RightValue = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsAiming = false;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsCrouchAiming = false;

	// == Aim Off ==
	UPROPERTY(BlueprintReadWrite, Category = "Aim")
	float AimPitch = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Aim")
	float AimYaw = 0.f;
};
