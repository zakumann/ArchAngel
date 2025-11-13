// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("WeaponSkeletalMesh"));
	WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	WeaponSkeletalMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponSkeletalMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	WeaponSkeletalMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	WeaponSkeletalMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	WeaponSkeletalMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	WeaponSkeletalMesh->SetGenerateOverlapEvents(true);
	WeaponSkeletalMesh->SetupAttachment(GetRootComponent());
	WeaponSkeletalMesh->SetHiddenInGame(true);
}

