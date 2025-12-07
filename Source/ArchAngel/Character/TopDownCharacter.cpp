// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchAngel/Character/TopDownCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ATopDownCharacter::ATopDownCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Spring Arm
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	// Distance above player
	CameraBoom->TargetArmLength = 600.0f;

	// Top-down angle (straight down is -90)
	CameraBoom->SetWorldRotation(FRotator(-60.f, 0.f, 0.f));

	// Do not rotate with character
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;

	// No camera collision (prevents ugly pushing distance)
	CameraBoom->bDoCollisionTest = false;

	// Create camera
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

// Called when the game starts or when spawned
void ATopDownCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
}

// Called every frame
void ATopDownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateToMouseCursor();
}

// Called to bind functionality to input
void ATopDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownCharacter::Move);
	}
}

void ATopDownCharacter::Move(const FInputActionValue& Value)
{
	FVector2D Movement = Value.Get<FVector2D>();

	// W/S = forward vector axis (Y)
	// A/D = right vector axis (X)
	AddMovementInput(FVector::ForwardVector, Movement.Y);
	AddMovementInput(FVector::RightVector, Movement.X);
}

void ATopDownCharacter::RotateToMouseCursor()
{
	APlayerController* PC = Cast<APlayerController>(Controller);
	if (!PC) return;

	// Trace under the mouse cursor
	FVector WorldLocation, WorldDirection;
	if (PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		// Plane trace for ground (Z = character Z)
		FVector Start = WorldLocation;
		FVector End = Start + (WorldDirection * 10000.0f);

		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECC_Visibility,
			Params))
		{
			FVector Target = HitResult.Location;
			FVector Direction = Target - GetActorLocation();
			Direction.Z = 0.f;     // flat rotate
			FRotator NewRot = Direction.Rotation();

			SetActorRotation(NewRot);
		}
	}
}
