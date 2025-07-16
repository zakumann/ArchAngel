// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ADoorActor::ADoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Frame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame"));
    RootComponent = Frame;

    Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
    Door->SetupAttachment(Frame);

    TimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComp"));

}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	
	TimelineCallback.BindDynamic(this, &ADoorActor::HandleProgress);
    if (DoorCurve)
    {
        TimelineComp->AddInterpFloat(DoorCurve, TimelineCallback);
    }
}

void ADoorActor::HandleProgress(float Value)
{
	FRotator R(0.f, FMath::Lerp(0.f, 90.f, Value), 0.f);
    Door->SetRelativeRotation(R);
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorActor::OnInteract_Implementation(AActor *Interactor)
{
	if (TimelineComp->IsPlaying())
        TimelineComp->Reverse();
    else
        TimelineComp->Play();
}
