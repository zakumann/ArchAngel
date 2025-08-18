#include "DoorActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerCharacter.h"
#include "TimerManager.h"

ADoorActor::ADoorActor()
{
	PrimaryActorTick.bCanEverTick = true;

	DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMesh"));
	SetRootComponent(DoorFrameMesh);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(DoorFrameMesh);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 200.f));
	TriggerBox->SetCollisionProfileName("Trigger");
}

void ADoorActor::BeginPlay()
{
	Super::BeginPlay();

	ClosedRotation = DoorMesh->GetRelativeRotation();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ADoorActor::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ADoorActor::OnOverlapEnd);
}

void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator CurrentRot = DoorMesh->GetRelativeRotation();
	FRotator NewRot = FMath::RInterpTo(CurrentRot, bIsOpen ? TargetOpenRotation : ClosedRotation, DeltaTime, OpenSpeed);
	DoorMesh->SetRelativeRotation(NewRot);
}

void ADoorActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		OpenDoor(Player);
		GetWorldTimerManager().ClearTimer(CloseTimerHandle);
	}
}

void ADoorActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		// 일정 시간 뒤에 자동 닫힘
		GetWorldTimerManager().SetTimer(CloseTimerHandle, this, &ADoorActor::CloseDoor, 2.f, false);
	}
}

void ADoorActor::OpenDoor(AActor* PlayerActor)
{
	if (!PlayerActor) return;

	// 플레이어와 문의 상대 위치 계산
	FVector DoorLocation = DoorFrameMesh->GetComponentLocation();
	FVector DoorForward = DoorFrameMesh->GetForwardVector(); // 문의 앞 방향
	FVector PlayerLocation = PlayerActor->GetActorLocation();

	// 플레이어 → 문 방향 벡터
	FVector ToPlayer = (PlayerLocation - DoorLocation).GetSafeNormal();

	// 앞/뒤 판별 : 문 Forward와 플레이어 위치 벡터의 내적
	float Dot = FVector::DotProduct(DoorForward, ToPlayer);

	if (Dot > 0)
	{
		// 플레이어가 문 앞쪽
		TargetOpenRotation = ClosedRotation + FRotator(0.f, -OpenAngle, 0.f);
	}
	else
	{
		// 플레이어가 문 뒤쪽
		TargetOpenRotation = ClosedRotation + FRotator(0.f, OpenAngle, 0.f);
	}

	bIsOpen = true;
}

void ADoorActor::CloseDoor()
{
	bIsOpen = false;
}