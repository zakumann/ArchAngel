#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorActor.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class ARCHANGEL_API ADoorActor : public AActor
{
	GENERATED_BODY()

public:
	ADoorActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* DoorFrameMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* TriggerBox;

	// Door rotation
	FRotator ClosedRotation;
	FRotator TargetOpenRotation;

	UPROPERTY(EditAnywhere, Category = "Door")
	float OpenAngle = 90.f; // 문 열리는 각도

	UPROPERTY(EditAnywhere, Category = "Door")
	float OpenSpeed = 20.f; // 열고 닫히는 속도 (Lerp 보간)

	bool bIsOpen = false;

	FTimerHandle CloseTimerHandle;

	// Functions
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void OpenDoor(AActor* PlayerActor);
	void CloseDoor();

public:
	virtual void Tick(float DeltaTime) override;
};