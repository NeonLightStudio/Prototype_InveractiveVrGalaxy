// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveVRGalaxy.h"
#include "Celestial/SolarSystem.h"
#include "GalaxyGameMode.h"

AGalaxyGameMode::AGalaxyGameMode(): m_bSpawnOrbit(false), m_bDespawnOrbit(false)
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

void AGalaxyGameMode::DespawnOrbit() const
{
	for (TActorIterator<ASolarSystem> itr(Super::GetWorld()); itr; ++itr)
	{
		ASolarSystem *system = *itr;
		if (system == nullptr)
		{
			continue;
		}
		system->MarkForDespawn();
		//system->DespawnSystem();
		//system->Destroy();
	}
}

#if WITH_EDITOR
void AGalaxyGameMode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName name = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (name == GET_MEMBER_NAME_CHECKED(AGalaxyGameMode, m_bSpawnOrbit))
	{
		this->DespawnOrbit();
		this->SpawnOrbit(FVector(-765.0f, -385.0f, 265.0f));
		this->m_bSpawnOrbit = false;
	}
	if (name == GET_MEMBER_NAME_CHECKED(AGalaxyGameMode, m_bDespawnOrbit))
	{
		this->DespawnOrbit();
		this->m_bDespawnOrbit = false;
	}
}
#endif