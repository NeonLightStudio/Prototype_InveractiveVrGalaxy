// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveVRGalaxy.h"
#include "Celestial/SolarSystem.h"
#include "GalaxyGameMode.h"

AGalaxyGameMode::AGalaxyGameMode()
{
	static ConstructorHelpers::FClassFinder<ASolarSystem> System(TEXT("/Game/VirtualRealityBP/Blueprints/BP_SolarSystem_Mini"));
	this->m_OrbitClass = System.Class;

	static ConstructorHelpers::FClassFinder<APawn> Pawn(TEXT("/Game/VirtualRealityBP/Blueprints/BP_DebugCharacter"));
	Super::DefaultPawnClass = Pawn.Class;
}

void AGalaxyGameMode::SpawnOrbit(const FVector& location)
{
	Super::GetWorld()->SpawnActor<ASolarSystem>(this->m_OrbitClass, location, FRotator(0.0f));
}

void AGalaxyGameMode::DespawnOrbit()
{
	for(TActorIterator<ASolarSystem> itr(Super::GetWorld()); itr; ++itr)
	{
		ASolarSystem *system = *itr;
		if (system == nullptr)
		{
			continue;
		}
		system->Destroy();
	}
}
