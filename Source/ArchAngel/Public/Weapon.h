// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class ARCHANGEL_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	/** Damage per shot */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
    float Power = 20.f;

    /** Current ammo in magazine */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
    int32 Ammo = 30;

    /** Maximum magazine capacity */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
    int32 MaxAmmo = 30;

    /** Reserve ammo for reloading */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
    int32 ReserveAmmo = 90;

    /** Fire the weapon */
    UFUNCTION(BlueprintCallable, Category="Weapon")
    virtual void Fire();

    /** Reload the weapon */
    UFUNCTION(BlueprintCallable, Category="Weapon")
    void Reload();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int32 BurstCount = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
