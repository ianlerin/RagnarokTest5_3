// Copyright Epic Games, Inc. All Rights Reserved.

#include "RagnarokTest5_3GameMode.h"
#include "RagnarokTest5_3Character.h"
#include "UObject/ConstructorHelpers.h"

ARagnarokTest5_3GameMode::ARagnarokTest5_3GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
