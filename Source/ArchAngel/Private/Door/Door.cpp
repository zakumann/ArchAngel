#include "Door.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Character/PlayerCharacter.h"   // Your custom character

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
	TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(SceneRoot);

	ClosedRotation = FRotator::ZeroRotator;
	OpenRotation = FRotator(0.f, 90.f, 0.f);   // Rotate 90¡Æ around Yaw by default
	OpenCloseSpeed = 120.f;                    // Degrees per second
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	// Initialize door rotation as closed
	DoorMesh->SetRelativeRotation(ClosedRotation);

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnTriggerBoxBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnTriggerBoxEndOverlap);
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Decide target rotation based on whether player is in range
	const FRotator TargetRotation = bPlayerInRange ? OpenRotation : ClosedRotation;

	// Current rotation (relative)
	const FRotator CurrentRotation = DoorMesh->GetRelativeRotation();

	// Interp towards target
	const float InterpSpeed = OpenCloseSpeed;
	const FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, InterpSpeed);

	DoorMesh->SetRelativeRotation(NewRotation);
}

void ADoor::OnTriggerBoxBeginOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (!OtherActor)
	{
		return;
	}

	// Only react to your PlayerCharacter
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		OpenDoor();
	}
}

void ADoor::OnTriggerBoxEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	if (!OtherActor)
	{
		return;
	}

	// Only react to your PlayerCharacter
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		CloseDoor();
	}
}

void ADoor::OpenDoor()
{
	bPlayerInRange = true;
}

void ADoor::CloseDoor()
{
	bPlayerInRange = false;
}