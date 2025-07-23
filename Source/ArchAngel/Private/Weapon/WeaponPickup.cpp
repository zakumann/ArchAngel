// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponPickup.h"
#include "PlayerCharacter.h"

// Sets default values
AWeaponPickup::AWeaponPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = MeshComp;

    ProximityBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Proximity"));
    ProximityBox->SetupAttachment(RootComponent);
    ProximityBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ProximityBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    ProximityBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    ProximityBox->OnComponentBeginOverlap.AddDynamic(this, &AWeaponPickup::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AWeaponPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponPickup::OnOverlapBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (APlayerCharacter* PC = Cast<APlayerCharacter>(OtherActor))
    {
        PC->NotifyNearbyPickup(this);
    }
}

void AWeaponPickup::OnOverlapEnd(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* PC = Cast<APlayerCharacter>(OtherActor))
    {
        PC->NotifyNearbyPickup(nullptr);
    }
}

// Called every frame
void AWeaponPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponPickup::GrantPickup(APlayerCharacter *PC)
{
	if (!WeaponClass || !PC) return;

    PC->GiveWeapon(WeaponClass);
    Destroy();
}
