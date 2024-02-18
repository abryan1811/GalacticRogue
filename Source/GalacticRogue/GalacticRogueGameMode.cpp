// Copyright Epic Games, Inc. All Rights Reserved.

#include "GalacticRogueGameMode.h"
#include "GalacticRogueCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGalacticRogueGameMode::AGalacticRogueGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
