// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "WeaponPickup.generated.h"

UCLASS()
class ARCHANGEL_API AWeaponPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere)
    UBoxComponent* ProximityBox;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AWeapon> WeaponClass;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GrantPickup(class APlayerCharacter* PC);
};
