// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class ARCHANGEL_API AGameControllerBase : public APlayerController
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Input setup
	virtual void SetupInputComponent() override;

};
