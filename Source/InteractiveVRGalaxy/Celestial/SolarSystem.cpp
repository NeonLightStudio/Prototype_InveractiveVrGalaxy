// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveVRGalaxy.h"
#include "SolarSystem.h"

// Sets default values
ASolarSystem::ASolarSystem() : m_TimeScale(1.0f), m_RadiusScale(1.0f), m_OrbitDistanceScale(1.0f),
m_CenterOffset(0.0f), m_CenterActor(nullptr), bScaleUpdateRequired(false)
{
	this->m_Root = UObject::CreateDefaultSubobject<USceneComponent>(TEXT("SolarSystemRoot"));
	Super::RootComponent = this->m_Root;

	Super::PrimaryActorTick.bCanEverTick = true;
}

void ASolarSystem::DespawnSystem()
{
	TArray<AActor*> systemActors, actors;

	this->GetAttachedActors(systemActors);
	for (AActor *next : systemActors)
	{
		if (next == nullptr)
		{
			continue;
		}
		next->GetAttachedActors(actors);
		for (AActor *actor : actors)
		{
			if (actor == nullptr)
			{
				continue;
			}
			actor->Destroy();
		}
		next->Destroy();
	}
	this->m_AttachedBodies.Empty();
}

// Called when the game starts or when spawned
void ASolarSystem::BeginPlay()
{
	Super::BeginPlay();

	if (Super::GetWorld() == nullptr)
	{
		return;
	}
	this->SpawnBodies(this, this->m_CelestialBodies);

	AActor *center = Super::GetWorld()->SpawnActor(this->m_Center);
	if (center)
	{
		this->m_CenterActor = (ACelestialBody*)center;
		center->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		this->m_AttachedBodies.Add(this->m_CenterActor);
	}
	this->bScaleUpdateRequired = true;
}

void ASolarSystem::SpawnBodies(AActor *parent, const TArray<TSubclassOf<ACelestialBody>>& bodies)
{
	check(parent);
	// Add all the bodies in reverse order since AttachToActor works using a push-to-front mechanic
	for (int i = bodies.Num() - 1; i >= 0; i--)
	{
		ACelestialBody *actor = Super::GetWorld()->SpawnActor<ACelestialBody>(bodies[i]);
		if (actor == nullptr)
		{
			continue;
		}
		actor->AttachToActor(parent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		this->m_AttachedBodies.Add(actor);

		actor->SetSunLocation(Super::GetActorLocation());

		const TArray<TSubclassOf<ACelestialBody>>& satellites = actor->GetSatellites();
		if (satellites.Num() > 0)
		{
			this->SpawnBodies(actor, satellites);
		}
	}
}

#if WITH_EDITOR
void ASolarSystem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName name = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (name == GET_MEMBER_NAME_CHECKED(ASolarSystem, m_RadiusScale) || name == GET_MEMBER_NAME_CHECKED(ASolarSystem, m_OrbitDistanceScale))
	{
		this->bScaleUpdateRequired = true;
	}
}
#endif

// Called every frame
void ASolarSystem::Tick(float delta)
{
	Super::Tick(delta);

	for (AActor *actor : this->m_AttachedBodies)
	{
		ACelestialBody *body = Cast<ACelestialBody>(actor);
		if(body == nullptr)
		{
			continue;
		}
		if (this->bScaleUpdateRequired)
		{
			body->SetScale(this->m_RadiusScale);
			if (body == this->m_CenterActor)
			{
				this->m_CenterOffset = body->GetRadiusWithScale();
			}
		}
		body->Move(this->m_CenterActor, this->m_TimeScale, this->m_OrbitDistanceScale, delta);
	}
	if (this->bScaleUpdateRequired)
	{
		this->bScaleUpdateRequired = false;
	}
}

