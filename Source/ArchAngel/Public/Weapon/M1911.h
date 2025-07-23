// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "M1911.generated.h"

/**
 * 
 */
UCLASS()
class ARCHANGEL_API AM1911 : public AWeapon
{
	GENERATED_BODY()
	
public:
    AM1911()
    {
        Ammo = MaxAmmo = 7;
        ReserveAmmo = 28;
    }
};
