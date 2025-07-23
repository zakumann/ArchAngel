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
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void UpdateAnimationProperties();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool bIsAccelerating = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bIsWalking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bIsRunning = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement)
    bool bIsSprinting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasWeapon = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float WalkSpeedThreshold = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float RunSpeedThreshold = 600.f;

	UPROPERTY()
    APawn* Pawn = nullptr;


	UPROPERTY()
    class APlayerCharacter* PlayerCharacter = nullptr;
};
