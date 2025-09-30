// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/AAGameModeBase.h"
#include "Character/PlayerCharacter.h"
#include "PlayerController/GameControllerBase.h"

AAAGameModeBase::AAAGameModeBase()
{
	// Set default pawn (your PlayerCharacter)
	DefaultPawnClass = APlayerCharacter::StaticClass();

	// Set custom PlayerController
	PlayerControllerClass = AGameControllerBase::StaticClass();
}
