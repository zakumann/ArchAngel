// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "TimerManager.h"
#include "Engine/World.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Fire()
{
	if (Ammo <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No ammo! Please reload."));
        return;
    }

    Ammo--;
    UE_LOG(LogTemp, Log, TEXT("Fired. Ammo left: %d"), Ammo);
}

void AWeapon::Reload()
{
	if (Ammo >= MaxAmmo)
    {
        UE_LOG(LogTemp, Warning, TEXT("Clip is already full."));
        return;
    }
    if (ReserveAmmo <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No reserve ammo."));
        return;
    }

    int32 Needed = MaxAmmo - Ammo;
    int32 Deduct = FMath::Min(Needed, ReserveAmmo);

    Ammo += Deduct;
    ReserveAmmo -= Deduct;

    UE_LOG(LogTemp, Log, TEXT("Reloaded. Ammo: %d | Reserve: %d"), Ammo, ReserveAmmo);
}