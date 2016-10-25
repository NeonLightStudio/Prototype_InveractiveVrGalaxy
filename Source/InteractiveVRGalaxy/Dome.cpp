// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveVRGalaxy.h"

#include "Dome.h"

// Sets default values
ADome::ADome() : m_DomeState(EDomeState::Opaque)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->m_Mesh = UObject::CreateDefaultSubobject<UDomeMeshComponent>(TEXT("DomeMesh"));
	Super::RootComponent = this->m_Mesh;
}

// Called when the game starts or when spawned
void ADome::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADome::Tick(float delta)
{
	Super::Tick(delta);
}

void ADome::NextDomeState()
{
	switch (this->m_DomeState)
	{
		case EDomeState::Opaque:
			this->m_DomeState = EDomeState::Transparent;
			this->m_Mesh->SetTransparent();
			break;

		case EDomeState::Transparent:
			this->m_DomeState = EDomeState::Opaque;
			this->m_Mesh->SetOpaque();
			break;
	}
}


