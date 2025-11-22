// Fill out your copyright notice in the Description page of Project Settings.

#include "Door/Door.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(SceneRoot);
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionObjectType(ECC_WorldDynamic);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerBox->SetBoxExtent(FVector(50.f, 50.f, 100.f));

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(SceneRoot);

	ClosedRotation = FRotator::ZeroRotator;

	// 180° open in each direction (Yaw axis by default)
	OpenRotationA = FRotator(0.f, 135.f, 0.f);   // one side
	OpenRotationB = FRotator(0.f, -135.f, 0.f);  // other side

	OpenCloseSpeed = 500.f; // degrees per second
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	// Start closed
	DoorMesh->SetRelativeRotation(ClosedRotation);
	TargetRotation = ClosedRotation;

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnTriggerBoxBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnTriggerBoxEndOverlap);
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FRotator CurrentRotation = DoorMesh->GetRelativeRotation();

	// Smoothly interpolate to target
	const FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, OpenCloseSpeed);

	DoorMesh->SetRelativeRotation(NewRotation);
}

void ADoor::OnTriggerBoxBeginOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		bPlayerInRange = true;
		OpenDoor(Player);
	}
}

void ADoor::OnTriggerBoxEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (!OtherActor) return;

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		bPlayerInRange = false;
		CloseDoor();
	}
}

void ADoor::OpenDoor(class APlayerCharacter* Player)
{
	bPlayerInRange = true;

	if (!Player)
	{
		TargetRotation = OpenRotationA;
		return;
	}

	FVector Velocity = Player->GetVelocity();
	Velocity.Z = 0.f;

	if (Velocity.IsNearlyZero())
	{
		TargetRotation = OpenRotationA;
		return;
	}

	Velocity.Normalize();

	FVector DoorForward = GetActorForwardVector();
	DoorForward.Z = 0.f;
	DoorForward.Normalize();

	FVector DoorRight = GetActorRightVector();
	DoorRight.Z = 0.f;
	DoorRight.Normalize();

	const float ForwardDot = FVector::DotProduct(DoorForward, Velocity);

	const float RightDot = FVector::DotProduct(DoorRight, Velocity);

	FRotator ChosenOpenRotation = OpenRotationA;

	if (ForwardDot > 0.f)
	{
		ChosenOpenRotation = OpenRotationA;
	}
	else
	{
		ChosenOpenRotation = OpenRotationB;
	}

	TargetRotation = ChosenOpenRotation;
}

void ADoor::CloseDoor()
{
	bPlayerInRange = false;
	TargetRotation = ClosedRotation;
}
