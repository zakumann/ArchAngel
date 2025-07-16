// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "InteractInterface.h"
#include "DoorActor.generated.h"

UCLASS()
class ARCHANGEL_API ADoorActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Frame;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Door;

    UPROPERTY(VisibleAnywhere)
    UTimelineComponent* TimelineComp;

    UPROPERTY(EditAnywhere)
    UCurveFloat* DoorCurve;

    FOnTimelineFloat TimelineCallback;

    UFUNCTION()
    void HandleProgress(float Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteract_Implementation(AActor* Interactor) override;

};
