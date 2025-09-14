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
#include "UMG/SlowMoWidget.h"
#include "MainAnimInstance.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationYaw = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bUseControllerDesiredRotation = false;
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

    AnimInstanceRef = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance());

    if (APlayerController* PC = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(InputMappingContext, 0);
        }
    }

    DefaultFOV = FollowCamera->FieldOfView;
    SlowMoRemaining = SlowMoTotalTime;

    if (SlowMoWidgetClass)
    {
        APlayerController* PC = Cast<APlayerController>(Controller);
        if (PC)
        {
            SlowMoWidgetInstance = CreateWidget<USlowMoWidget>(PC, SlowMoWidgetClass);
            if (SlowMoWidgetInstance)
            {
                SlowMoWidgetInstance->AddToViewport();
            }
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
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerCharacter::OnMoveCompleted);

        // Bind the look input action
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

        EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &APlayerCharacter::StartAiming);
        EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopAiming);

        // Bind Sprint
        EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StartWalk);
        EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopWalk);

        EnhancedInputComponent->BindAction(SlowAction, ETriggerEvent::Started, this, &APlayerCharacter::ToggleSlowMo);
        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Fire);

        //Crouch
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::HandleCrouchToggle);

        //Interact
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);

        //Dodge
        EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &APlayerCharacter::Dodge);
    }
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateAim(DeltaTime);

    // Update SlowMo progress bar
    if (SlowMoWidgetInstance)
    {
        float Percent = (SlowMoTotalTime > 0) ? SlowMoRemaining / SlowMoTotalTime : 0.f;
        SlowMoWidgetInstance->UpdateSlowMoBar(Percent);
    }
}

// ========== MOVEMENT ==========
void APlayerCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();

    // 🔹 Save it here so Dodge() can read it later
    CachedMoveInput = MovementVector;

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(ForwardDirection, MovementVector.Y);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(RightDirection, MovementVector.X);
}

// ========== LOOK ==========
void APlayerCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();
    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
}

// ========== AIM ==========
void APlayerCharacter::StartAiming()
{
    bIsAiming = true;

    // Switch to controller rotation
    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->bUseControllerDesiredRotation = true;

    CameraBoom->TargetArmLength = 50.f;

    // Cancel sprint when aiming
    bIsWalking = true;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    if (bIsCrouching)
    {
        CameraBoom->TargetArmLength = 100.f;
    }
}
void APlayerCharacter::StopAiming()
{
    bIsAiming = false;

    // Back to orient rotation to movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->bUseControllerDesiredRotation = false;

    CameraBoom->TargetArmLength = 100.f;

    bIsWalking = false;
    GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
}

// ========== SLOW MOTION SYSTEM ==========
void APlayerCharacter::ToggleSlowMo()
{
    bIsInSlowMo ? StopSlowMo() : StartSlowMo();
}

void APlayerCharacter::StartSlowMo()
{
    if (bIsInSlowMo || SlowMoRemaining <= 0.f) return;

    UGameplayStatics::SetGlobalTimeDilation(this, 0.25f);
    bIsInSlowMo = true;

    GetWorldTimerManager().SetTimer(SlowMoTimerHandle, [this]() { StopSlowMo(); }, SlowMoRemaining, false);
}

void APlayerCharacter::StopSlowMo()
{
    if (!bIsInSlowMo) return;

    UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
    bIsInSlowMo = false;
    SlowMoRemaining = 0.f;

    // recharge after delay
    GetWorldTimerManager().SetTimer(SlowMoRechargeHandle, [this]()
        {
            GetWorldTimerManager().SetTimer(SlowMoRechargeHandle, [this]()
                {
                    SlowMoRemaining = FMath::Min(SlowMoRemaining + SlowMoRechargeRate, SlowMoTotalTime);
                    if (SlowMoRemaining >= SlowMoTotalTime)
                        GetWorldTimerManager().ClearTimer(SlowMoRechargeHandle);
                }, 1.f, true);
        }, SlowMoRechargeDelay, false);
}

// ========== SPRINT ==========
void APlayerCharacter::StartWalk()
{
    if (!bIsAiming)
    {
        if (bIsCrouched)
        {
            UnCrouch();
            bIsCrouching = false;
        }
        bIsWalking = true;
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed; // Use WalkSpeed here
    }
}

void APlayerCharacter::StopWalk()
{
    bIsWalking = false;
    GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
}
// ========== INTERACT ==========
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

// ========== CROUCH ==========
void APlayerCharacter::HandleCrouchToggle()
{
    if (bIsCrouching)
    {
        // Already crouching → stand up
        UnCrouch();
        bIsCrouching = false;

        // Restore normal walk speed
        GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
    }
    else
    {
        // Not crouching → crouch down
        Crouch();
        bIsCrouching = true;

        // Optional: reduce speed while crouching
        GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
        bIsAiming = false;
    }

    // Cancel sprint if crouching
    if (bIsCrouching && bIsWalking)
    {
        bIsWalking = false;
    }
}

// ========== FIRE / RELOAD ==========
void APlayerCharacter::Fire()
{
}


void APlayerCharacter::ReloadWeapon()
{
}

// ========== AIM CAMERA ==========
void APlayerCharacter::UpdateAim(float DeltaTime)
{
    AimArmLength = FMath::FInterpTo(AimArmLength, TargetAimArmLength, DeltaTime, AimInterpSpeed);
    CameraBoom->TargetArmLength = AimArmLength;

    float CurrentFOV = FollowCamera->FieldOfView;
    float DesiredFOV = bIsAiming ? AimFOV : DefaultFOV;
    FollowCamera->SetFieldOfView(FMath::FInterpTo(CurrentFOV, DesiredFOV, DeltaTime, AimInterpSpeed));
}
// ========== DODGE ==========
void APlayerCharacter::Dodge()
{
    if (!bCanDodge) return;

    // Build forward/right from *mouse/camera yaw*
    const FRotator ControlRot = Controller ? Controller->GetControlRotation() : GetActorRotation();
    const FRotator YawRot(0.f, ControlRot.Yaw, 0.f);
    const FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
    const FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

    FVector DodgeDir = (Forward * CachedMoveInput.Y + Right * CachedMoveInput.X);

    // Default forward if no input
    if (DodgeDir.IsNearlyZero())
    {
        DodgeDir = Forward;
    }

    DodgeDir.Normalize();

    // Launch
    LaunchCharacter(DodgeDir * DodgeStrength + FVector(0.f, 0.f, UpwardBoostZ), true, true);

    // Choose enum for animation
    EDodgeDirection DirEnum = EDodgeDirection::Forward;

    // Tell AnimInstance we are dodging
    if (UMainAnimInstance* AnimInst = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance()))
    {
        AnimInst->bIsDodging = true;
        AnimInst->DodgeDirection = DodgeDir;

        const float ForwardDot = FVector::DotProduct(DodgeDir, Forward);
        const float RightDot = FVector::DotProduct(DodgeDir, Right);

        if (ForwardDot > 0.7f) DirEnum = EDodgeDirection::Forward;
        else if (ForwardDot < -0.7f) DirEnum = EDodgeDirection::Backward;
        else if (RightDot > 0.7f) DirEnum = EDodgeDirection::Right;
        else if (RightDot < -0.7f) DirEnum = EDodgeDirection::Left;
    }

    StartSlowMo();

    // Lockout
    bCanDodge = false;
    GetWorldTimerManager().SetTimer(DodgeCooldownTimerHandle, [this]()
        {
            bCanDodge = true;
            if (UMainAnimInstance* AnimInst = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance()))
            {
                AnimInst->bIsDodging = false;
            }
        }, DodgeCooldown, false);

    bIsAiming = false;

    UE_LOG(LogTemp, Warning, TEXT("CachedMoveInput X=%f Y=%f"), CachedMoveInput.X, CachedMoveInput.Y);
}

void APlayerCharacter::OnMoveCompleted(const FInputActionValue& Value)
{
    CachedMoveInput = FVector2D::ZeroVector;
}