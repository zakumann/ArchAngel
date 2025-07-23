// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Compact_Pistol.generated.h"

/**
 * 
 */
UCLASS()
class ARCHANGEL_API ACompact_Pistol : public AWeapon
{
	GENERATED_BODY()
	
public:
	ACompact_Pistol()
	{
		Ammo = MaxAmmo = 12;
        ReserveAmmo = 36;
	}
};
