// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom);

    bUseControllerRotationYaw = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    if (APlayerController* PC = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(InputMapping, 0);
        }
    }
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
        EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
        EnhancedInput->BindAction(AimAction, ETriggerEvent::Started, this, &APlayerCharacter::StartAiming);
        EnhancedInput->BindAction(AimAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopAiming);
        EnhancedInput->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerCharacter::Fire);
        EnhancedInput->BindAction(SlowMoAction, ETriggerEvent::Started, this, &APlayerCharacter::StartSlowMo);
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::StartSprinting);
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprinting);
    }
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
    FVector2D Movement = Value.Get<FVector2D>();
    AddMovementInput(GetActorForwardVector(), Movement.Y);
    AddMovementInput(GetActorRightVector(), Movement.X);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxis = Value.Get<FVector2D>();
    AddControllerYawInput(LookAxis.X);
    AddControllerPitchInput(LookAxis.Y);
}

void APlayerCharacter::StartAiming()
{
    bIsAiming = true;
    CameraBoom->TargetArmLength = 200.f;
}

void APlayerCharacter::StopAiming()
{
    bIsAiming = false;
    CameraBoom->TargetArmLength = 300.f;
}

void APlayerCharacter::Fire()
{
    // Call weapon class Fire() here
}

void APlayerCharacter::StartSlowMo()
{
    float CurrentDilation = UGameplayStatics::GetGlobalTimeDilation(this);
    UGameplayStatics::SetGlobalTimeDilation(this, (CurrentDilation < 1.f) ? 1.f : 0.25f);
}

void APlayerCharacter::StartSprinting()
{
    if (!bIsAiming) // Optional: block sprinting while aiming
    {
        bIsSprinting = true;
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void APlayerCharacter::StopSprinting()
{
    bIsSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

