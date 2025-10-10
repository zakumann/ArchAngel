// This code copyright is owned by a third party. Redistribution, modification or use of this code is not permitted without prior written consent from the copyright holder.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

class UPoseSearchDatabase;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	class APlayerCharacter* PlayerCharacter;

	// Movement speed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float Speed;

	// Movement direction (for blendspace)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float Direction;
};
