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
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ATopDownCharacter::Fire);
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

void ATopDownCharacter::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire!"));
}

void ATopDownCharacter::RotateToMouseCursor()
{
	APlayerController* PC = Cast<APlayerController>(Controller);
	if (!PC) return;

	//Get the character's current height (Z-coordinate) to define the ground plane
	const float CharacterHeight = GetActorLocation().Z;

	// Define an infinite ground plane at the character's feet
	const FPlane GroundPlane(FVector(0.f, 0.f, CharacterHeight), FVector(0.f, 0.f, 1.f));

	// Trace under the mouse cursor
	FVector WorldLocation, WorldDirection;
	if (PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		// Calculate the intersection point between the mouse ray and the ground plane
		FVector IntersectionPoint = FMath::RayPlaneIntersection(WorldLocation, WorldDirection, GroundPlane);

		// Calculate the direction vector from the character to the intersection point
		FVector Direction = IntersectionPoint - GetActorLocation();

		// Keep the direction vector flat (only X and Y movement) for top-down rotation
		Direction.Z = 0.f;

		// If the direction vector is valid (not zero length), calculate and set the new rotation
		if (!Direction.IsNearlyZero())
		{
			FRotator NewRot = Direction.Rotation();
			SetActorRotation(NewRot);
		}
	}
}
