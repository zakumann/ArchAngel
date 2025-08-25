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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Movement and look functions
	void Move(const FInputActionValue& Value);
	void Dodge();
	// helper to clear cached input when Move finishes
	void OnMoveCompleted(const FInputActionValue& Value);

	// Look, aim, sprint, etc.
	void Look(const FInputActionValue& Value);
	void StartAiming();
	void StopAiming();
	void Fire();
	void ToggleSlowMo();
	void StartSprint();
	void StopSprint();
	void Interact();
	void ReloadWeapon();
	void HandleCrouchToggle();
	void RotateCharacterToCursor(float DeltaTime);
	void UpdateAim(float DeltaSeconds);

	// ==== Camera ====
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

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* DodgeAction;

	// ==== Movement ====
	bool bIsAiming = false;

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

	// Dodge setting
	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	float DodgeStrength = 1200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	float DodgeCooldown = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	float UpwardBoostZ = 200.f;

	bool bCanDodge = true;
	// cache last movement input from Move()
	FVector2D CachedMoveInput = FVector2D::ZeroVector;
	FTimerHandle DodgeCooldownTimerHandle;

	// ==== Slow Motion Pool ====
	UPROPERTY(EditDefaultsOnly, Category = "SlowMotion") 
	float SlowMoTotalTime = 3.0f; // 3s pool 

	UPROPERTY(EditDefaultsOnly, Category = "SlowMotion")
	float SlowMoRechargeDelay = 5.0f; 

	UPROPERTY(EditDefaultsOnly, Category = "SlowMotion")
	float SlowMoRechargeRate = 1.0f; // per sec 

	float SlowMoRemaining; 
	bool bIsInSlowMo = false;
	FTimerHandle SlowMoTimerHandle;
	FTimerHandle SlowMoRechargeHandle;

	void StartSlowMo();
	void StopSlowMo();

	// === UMG Widget ===
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> SlowMoWidgetClass;

	UPROPERTY()
	class USlowMoWidget* SlowMoWidgetInstance;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsSprinting = false;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	bool bIsCrouching = false;


};