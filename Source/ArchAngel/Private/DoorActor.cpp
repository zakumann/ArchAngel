// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

// Sets default values
ADoorActor::ADoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Frame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame"));
    RootComponent = Frame;

    Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
    Door->SetupAttachment(Frame);
    Door->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Door->SetCollisionObjectType(ECC_WorldStatic);
    Door->SetCollisionResponseToAllChannels(ECR_Block);
    Door->SetGenerateOverlapEvents(false);

    ProximityBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ProximityBox"));
    ProximityBox->InitBoxExtent(FVector(150.f, 55.f, 106.f));
    ProximityBox->SetupAttachment(Frame);
    ProximityBox->SetRelativeLocation(FVector(0.f, 0.f, 106.f));


    TimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComp"));
}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (DoorCurve)
    {
        FOnTimelineFloat Callback;
        Callback.BindDynamic(this, &ADoorActor::HandleProgress);
        TimelineComp->AddInterpFloat(DoorCurve, Callback);
        TimelineComp->SetLooping(false);
        TimelineComp->SetIgnoreTimeDilation(true);
    }

    ProximityBox->OnComponentBeginOverlap.AddDynamic(this, &ADoorActor::OnProximityBegin);
    ProximityBox->OnComponentEndOverlap.AddDynamic(this, &ADoorActor::OnProximityEnd);
}

void ADoorActor::HandleProgress(float Value)
{
	float Yaw = Value * -90.f * OpenDirectionFactor;
    Door->SetRelativeRotation(FRotator(0.f, Yaw, 0.f));
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    TimelineComp->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
}

void ADoorActor::OnInteract_Implementation(AActor *Interactor)
{
	if (!bPlayerInRange || !Interactor) return;

    if (!bIsOpen)
    {
        // Determine opening direction only when opening
        FVector ToPlayer = Interactor->GetActorLocation() - GetActorLocation();
        FVector Forward = Frame->GetForwardVector();
        OpenDirectionFactor = (FVector::DotProduct(Forward, ToPlayer) > 0.f) ? 1.f : -1.f;

        TimelineComp->PlayFromStart();
        bIsOpen = true;
    }
    else
    {
        // Reuse the same OpenDirectionFactor when closing
        TimelineComp->Reverse();
        bIsOpen = false;
    }
}

void ADoorActor::OnProximityBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
    {
        bPlayerInRange = true;
    }
}

void ADoorActor::OnProximityEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
    {
        bPlayerInRange = false;
    }
}