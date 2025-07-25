// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Set mesh as root so attachment works by default
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;
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
        UE_LOG(LogTemp, Warning, TEXT("No ammo!"));
        return;
    }
    Ammo--;
    UE_LOG(LogTemp, Log, TEXT("Fired. %d/%d"), Ammo, ReserveAmmo);
}

void AWeapon::Reload()
{
	 if (Ammo >= MaxAmmo || ReserveAmmo <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot reload."));
        return;
    }
    int32 Needed = MaxAmmo - Ammo;
    int32 Deduct = FMath::Min(Needed, ReserveAmmo);
    Ammo += Deduct;
    ReserveAmmo -= Deduct;
    UE_LOG(LogTemp, Log, TEXT("Reloaded: %d | Reserve: %d"), Ammo, ReserveAmmo);
}