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

	// Reference to owning character
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	class APlayerCharacter* PlayerCharacter;

	// Movement speed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float Speed;

	// Movement direction (for blendspace)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float Direction = 0.f;

	// For Aim Offset
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aiming", meta = (AllowPrivateAccess = "true"))
	float AimYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aiming", meta = (AllowPrivateAccess = "true"))
	float AimPitch;
};
