// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Components/SphereComponent.h"
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
    USphereComponent* ProximitySphere;

    UPROPERTY(VisibleAnywhere)
    UTimelineComponent* TimelineComp;

    UPROPERTY(EditAnywhere, Category="Door")
    UCurveFloat* DoorCurve;

    bool bPlayerInRange = false;
    bool bIsOpen = false;
    float OpenDirectionFactor = 1.f;

    UFUNCTION()
    void HandleProgress(float Value);

    UFUNCTION()
    void OnProximityBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    UFUNCTION()
    void OnProximityEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteract_Implementation(AActor* Interactor) override;

};
