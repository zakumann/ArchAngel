// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DrawDebugHelpers.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    StandingCapsuleHalfHeight = 96.f;
    CrouchedCapsuleHalfHeight = 48.f;
    GetCapsuleComponent()->SetCapsuleHalfHeight(StandingCapsuleHalfHeight);

    // Create and attach the First-Person CameraComponent
    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
    FirstPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f)); // Position the camera
    FirstPersonCamera->bUsePawnControlRotation = true; // Rotate camera with controller
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    OriginalCameraOffset = FirstPersonCamera->GetRelativeLocation();
    TargetCameraOffset = OriginalCameraOffset;

    if (APlayerController* PC = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(InputMappingContext, 0);
        }
    }
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Cast to EnhancedInputComponent
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

        // Bind the look input action
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

        // Bind the Jump
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

        // Bind Sprint
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StartSprint);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprint);

        EnhancedInputComponent->BindAction(SlowAction, ETriggerEvent::Started, this, &APlayerCharacter::ToggleSlowMo);
        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Fire);

        // Interact
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);

        // Crouch
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::StartCrouch);
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopCrouch);
    }
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    float Target = bWantsCrouch ? 1.f : 0.f;
    CrouchAlpha = FMath::FInterpTo(CrouchAlpha, Target, DeltaTime, CrouchInterpSpeed);

    float NewHalf = FMath::Lerp(StandingCapsuleHalfHeight, CrouchedCapsuleHalfHeight, CrouchAlpha);
    float HalfDelta = NewHalf - GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
    GetCapsuleComponent()->SetCapsuleHalfHeight(NewHalf, true);

    // Move actor to stay ground-aligned
    AddActorWorldOffset(FVector(0, 0, -HalfDelta));

    FVector NewCam = FMath::Lerp(OriginalCameraRelative, OriginalCameraRelative + CrouchedCameraOffset, CrouchAlpha);
    FirstPersonCamera->SetRelativeLocation(NewCam);
}

void APlayerCharacter::Interact(const FInputActionValue& Value)
{
    FVector Start = FirstPersonCamera->GetComponentLocation();
    FVector End = Start + FirstPersonCamera->GetForwardVector() * 300.f;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

    DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Green : FColor::Red, false, 2.f);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();

    // Add movement input along the forward and right vectors
    AddMovementInput(GetActorForwardVector(), MovementVector.Y);
    AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();
    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
}

void APlayerCharacter::StartSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void APlayerCharacter::StopSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::ToggleSlowMo()
{
    if (!GetWorld()) return;
    bIsSlowMo = !bIsSlowMo;
    GetWorld()->GetWorldSettings()->SetTimeDilation(bIsSlowMo ? 0.25f : 1.f);
}

void APlayerCharacter::Fire()
{
}

void APlayerCharacter::StartCrouch(const FInputActionValue& Value)
{
    bWantsCrouch = true;
}

void APlayerCharacter::StopCrouch(const FInputActionValue& Value)
{
    // Set up collision query params and ignore ourselves
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);  // Correctly ignore the character :contentReference[oaicite:1]{index=1}

    FVector Start = GetCapsuleComponent()->GetComponentLocation();
    FVector End = Start + FVector(0, 0, UncrouchCheckHeight);
    FHitResult Hit;

    bool bBlocked = GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(UncrouchCheckRadius), Params);

    // Optional debug visualization
    DrawDebugCapsule(GetWorld(),
        End,
        /* half-height */ 1.0f,
        UncrouchCheckRadius,
        FQuat::Identity,
        bBlocked ? FColor::Red : FColor::Green,
        false, 1.0f
    );

    // Only allow uncrouch when not blocked
    if (!bBlocked)
    {
        bWantsCrouch = false;
    }
}