// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveVRGalaxy.h"
#include "DomeControl.h"
#include "Dome.h"

// Sets default values
ADomeControl::ADomeControl()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADomeControl::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADomeControl::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ADomeControl::NextDomeState()
{
	if (this->m_Dome == nullptr)
	{
		return;
	}
	if (this->m_Dome->GetDomeState() == EDomeState::Open)
	{
		this->m_Dome->SetDomeState(EDomeState::Close);
	}
	else
	{
		this->m_Dome->SetDomeState(EDomeState::Open);
	}
}
