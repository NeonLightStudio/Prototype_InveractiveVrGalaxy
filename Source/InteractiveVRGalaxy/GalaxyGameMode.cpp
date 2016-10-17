// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveVRGalaxy.h"
#include "GalaxyGameMode.h"

AGalaxyGameMode::AGalaxyGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> pawn(TEXT("/Game/VirtualRealityBP/Blueprints/BP_DebugCharacter"));
	Super::DefaultPawnClass = pawn.Class;
}



