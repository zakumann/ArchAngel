// This code copyright is owned by a third party. Redistribution, modification or use of this code is not permitted without prior written consent from the copyright holder.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARCHANGEL_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:

	// Reference to owning character
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	class APlayerCharacter* PlayerCharacter;

	// Movement speed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float Speed;

	// Movement direction (for blendspace)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	bool bHasWeapon;
};
