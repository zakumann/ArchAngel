// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Sauer220.generated.h"

/**
 * 
 */
UCLASS()
class ARCHANGEL_API ASauer220 : public AWeapon
{
	GENERATED_BODY()
	
public:
    ASauer220()
    {
        Ammo = MaxAmmo = 10;
        ReserveAmmo = 30;
    }
};
