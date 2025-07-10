// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
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

	// Input actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* SlowAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CrouchAction;

	// First-Person Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FirstPersonCamera;

	// Crouch state
	bool bWantsCrouch = false;
	float CrouchAlpha = 0.f;

	// Config
	UPROPERTY(EditAnywhere, Category = "Crouch") 
	float CrouchInterpSpeed = 4.f;

	UPROPERTY(EditAnywhere, Category = "Crouch") 
	float StandingCapsuleHalfHeight = 96.f;

	UPROPERTY(EditAnywhere, Category = "Crouch")
	float CrouchedCapsuleHalfHeight = 48.f;

	UPROPERTY(EditAnywhere, Category = "Crouch") 
	FVector CrouchedCameraOffset = FVector(0, 0, -40.f);

	UPROPERTY(EditAnywhere, Category = "Crouch")
	float UncrouchCheckRadius = 34.f;

	UPROPERTY(EditAnywhere, Category = "Crouch")
	float UncrouchCheckHeight = 50.f;

	FVector OriginalCameraRelative;

	UFUNCTION()
	void Interact(const FInputActionValue& Value);

	FVector OriginalCameraOffset;
	FVector TargetCameraOffset;

	// Movement and look functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartSprint();
	void StopSprint();
	void ToggleSlowMo();
	void Fire();

	// Crouch
	void StartCrouch(const FInputActionValue& Value);
	void StopCrouch(const FInputActionValue& Value);

	// Functional state
	bool bIsSlowMo = false;

	/** Movement Speed */
	float WalkSpeed = 600.f;
	float SprintSpeed = 1200.f;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
