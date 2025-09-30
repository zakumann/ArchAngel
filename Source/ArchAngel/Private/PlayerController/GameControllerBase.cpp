// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/GameControllerBase.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"

void AGameControllerBase::BeginPlay()
{
	// Ensure Enhanced Input subsystem is active
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// You can add a default InputMappingContext here if needed
		// Example: Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AGameControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();
}
