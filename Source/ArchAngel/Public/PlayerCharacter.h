// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ARCHANGEL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void StartAiming();
    void StopAiming();
    void Fire();
    void StartSlowMo();
    // Sprint input actions
    void StartSprinting();
    void StopSprinting();

private:
    UPROPERTY(VisibleAnywhere)
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere)
    class UCameraComponent* FollowCamera;

    UPROPERTY(EditDefaultsOnly, Category = Input)
    class UInputMappingContext* InputMapping;

    UPROPERTY(EditDefaultsOnly, Category = Input)
    class UInputAction* MoveAction;

    UPROPERTY(EditDefaultsOnly, Category = Input)
    class UInputAction* LookAction;

    UPROPERTY(EditDefaultsOnly, Category = Input)
    class UInputAction* AimAction;

    UPROPERTY(EditDefaultsOnly, Category = Input)
    class UInputAction* FireAction;

    UPROPERTY(EditDefaultsOnly, Category = Input)
    class UInputAction* SlowMoAction;

    UPROPERTY(EditDefaultsOnly, Category = Input)
    class UInputAction* SprintAction;

    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    float WalkSpeed = 300.f;

    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    float SprintSpeed = 600.f;


    bool bIsAiming;
    bool bIsSprinting;
};
