// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "InteractInterface.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationYaw = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 100.f;
    CameraBoom->bUsePawnControlRotation = false;

    // This is the important part: socket offset for side + height
    CameraBoom->SocketOffset = FVector(0.0f, 30.0f, 80.0f);

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    AimArmLength = CameraBoom->TargetArmLength;
    TargetAimArmLength = AimArmLength;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

    if (APlayerController* PC = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(InputMappingContext, 0);
        }
    }

    DefaultFOV = FollowCamera->FieldOfView;
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

        EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &APlayerCharacter::StartAiming);
        EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopAiming);

        // Bind Sprint
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StartSprint);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprint);

        EnhancedInputComponent->BindAction(SlowAction, ETriggerEvent::Started, this, &APlayerCharacter::ToggleSlowMo);
        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Fire);

        //Crouch
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::HandleCrouchToggle);

        //Interact
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
    }
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateAim(DeltaTime);
    RotateCharacterToCursor(DeltaTime);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(ForwardDirection, MovementVector.Y);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(RightDirection, MovementVector.X);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();
    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
}

void APlayerCharacter::StartAiming()
{
    bIsAiming = true;
    CameraBoom->TargetArmLength = 50.f;

    // Cancel sprint when aiming
    bIsSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    // Lock facing direction to camera, but still allow strafing
    GetCharacterMovement()->bOrientRotationToMovement = false;
    bUseControllerRotationYaw = true;
}
void APlayerCharacter::StopAiming()
{
    bIsAiming = false;
    CameraBoom->TargetArmLength = 100.f;

    // Back to movement-based rotation
    GetCharacterMovement()->bOrientRotationToMovement = true;
    bUseControllerRotationYaw = false;
}

void APlayerCharacter::ToggleSlowMo()
{
    float CurrentDilation = UGameplayStatics::GetGlobalTimeDilation(this);
    UGameplayStatics::SetGlobalTimeDilation(this, (CurrentDilation < 1.f) ? 1.f : 0.25f);
}

void APlayerCharacter::StartSprint()
{
    if (!bIsAiming)
    {
        if (bIsCrouched)
        {
            UnCrouch();
            bIsCrouching = false;
        }
        bIsSprinting = true;
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void APlayerCharacter::StopSprint()
{
    bIsSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::Interact()
{
    FVector Start = FollowCamera->GetComponentLocation();
    FVector End = Start + FollowCamera->GetForwardVector() * 500.f; // Adjust distance as needed
    FHitResult Hit;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        if (Hit.GetActor() && Hit.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
        {
            IInteractInterface::Execute_OnInteract(Hit.GetActor(), this);
        }
    }

    // debug line
    DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.f);
}

void APlayerCharacter::ReloadWeapon()
{

}

void APlayerCharacter::HandleCrouchToggle()
{
    if (bIsCrouching)
    {
        // Already crouching → stand up
        UnCrouch();
        bIsCrouching = false;

        // Restore normal walk speed
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    }
    else
    {
        // Not crouching → crouch down
        Crouch();
        bIsCrouching = true;

        // Optional: reduce speed while crouching
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    }

    // Cancel sprint if crouching
    if (bIsCrouching && bIsSprinting)
    {
        bIsSprinting = false;
    }
}

void APlayerCharacter::RotateCharacterToCursor(float DeltaTime)
{
    if (!bIsAiming) return;

    FRotator ControlRot = GetControlRotation();
    ControlRot.Pitch = 0.f; // Ignore pitch so character stays upright
    ControlRot.Roll = 0.f;

    // Smooth rotation toward camera yaw
    FRotator NewRot = FMath::RInterpTo(GetActorRotation(), ControlRot, DeltaTime, 15.f);
    SetActorRotation(NewRot);
}

void APlayerCharacter::Fire()
{
}

void APlayerCharacter::UpdateAim(float DeltaTime)
{
    AimArmLength = FMath::FInterpTo(AimArmLength, TargetAimArmLength, DeltaTime, AimInterpSpeed);
    CameraBoom->TargetArmLength = AimArmLength;

    float CurrentFOV = FollowCamera->FieldOfView;
    float DesiredFOV = bIsAiming ? AimFOV : DefaultFOV;
    FollowCamera->SetFieldOfView(FMath::FInterpTo(CurrentFOV, DesiredFOV, DeltaTime, AimInterpSpeed));
}
