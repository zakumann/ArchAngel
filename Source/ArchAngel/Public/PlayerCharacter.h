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

	UPROPERTY(BlueprintReadOnly, Category="Weapon")
    class AWeapon* CurrentWeapon = nullptr;

    /** Socket name on the character's skeletal mesh */
    UPROPERTY(EditDefaultsOnly, Category="Weapon")
    class FName WeaponSocketName = TEXT("WeaponSocket");
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Movement and look functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartAiming();
    void StopAiming();
	void Fire();
	void ToggleSlowMo();
	void StartSprint();
	void StopSprint();
	void Interact();
	void ReloadWeapon();
	void StartCrouch();
	void StopCrouch();

protected:
	UPROPERTY(VisibleAnywhere)
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere)
    class UCameraComponent* FollowCamera;
	// Input actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* SlowAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category="Input")
	class UInputAction* ReloadAction;

	bool bIsAiming = false;

	/** Movement Speed */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
    float WalkSpeed = 300.f;

    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    float SprintSpeed = 600.f;

	// Smooth aiming variables
    float AimArmLength;
    float TargetAimArmLength;
    float AimInterpSpeed = 12.f;
    float DefaultFOV = 90.f;
    float AimFOV = 65.f;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
    bool bIsSprinting = false;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	bool bIsCrouching = false;

public:
	// Called by the pickup actor
    void GiveWeapon(TSubclassOf<AWeapon> WeaponClass);
};