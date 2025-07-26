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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
    float Power = 20.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
    int32 Ammo = 30;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
    int32 MaxAmmo = 30;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
    int32 ReserveAmmo = 90;

    UFUNCTION(BlueprintCallable, Category="Weapon")
    virtual void Fire();

    UFUNCTION(BlueprintCallable, Category="Weapon")
    void Reload();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
