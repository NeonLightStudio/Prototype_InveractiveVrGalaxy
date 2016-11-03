// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveVRGalaxy.h"
#include "SolarSystem.h"

// Sets default values
ASolarSystem::ASolarSystem() : m_SizeScale(1.0f), m_DistanceScale(1.0f),
		bScaleUpdateRequired(false), m_CenterActor(nullptr), m_CenterOffset(0.0f), m_VelocityMultiplier(1.0f)
{
	this->m_Root = UObject::CreateDefaultSubobject<USceneComponent>(TEXT("SolarSystemRoot"));
	Super::RootComponent = this->m_Root;

	Super::PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASolarSystem::BeginPlay()
{
	Super::BeginPlay();

	check(Super::GetWorld());
	// Add all the bodies in reverse order since AttachToActor works using a push-to-front mechanic
	for(int i = this->m_CelestialBodies.Num() - 1; i >= 0; i--)
	{

		AActor *actor = Super::GetWorld()->SpawnActor(this->m_CelestialBodies[i]);
		if (actor)
		{
			actor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		}
	}
	AActor *center = Super::GetWorld()->SpawnActor(this->m_Center);
	if (center)
	{
		this->m_CenterActor = (ACelestialBody*)center;
		center->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
	this->bScaleUpdateRequired = true;

	Super::GetAttachedActors(this->m_AttachedActors);

	for (float i = 0; i < PI2; i += PI2 / 20)
	{
		float x = FMath::Cos(i), y = FMath::Sin(i);
		float r = ((ACelestialBody*)m_AttachedActors[1])->CalculateDistance(i);
		UE_LOG(LogClass, Log, TEXT("%f %f"), r, ((ACelestialBody*)m_AttachedActors[1])->CalculateVelocity(i));
		DrawDebugSphere(Super::GetWorld(), FVector(x * r * this->m_DistanceScale, y * r * this->m_DistanceScale, 0.0f), 50, 20, FColor::Red, true);
	}

}

#if WITH_EDITOR
void ASolarSystem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName name = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (name == GET_MEMBER_NAME_CHECKED(ASolarSystem, m_SizeScale) || name == GET_MEMBER_NAME_CHECKED(ASolarSystem, m_DistanceScale))
	{
		this->bScaleUpdateRequired = true;
	}
}
#endif

// Called every frame
void ASolarSystem::Tick(float delta)
{
	Super::Tick(delta);

	for (AActor *actor : this->m_AttachedActors)
	{
		if (!actor->IsA(ACelestialBody::StaticClass()))
		{
			continue;
		}
		ACelestialBody *body = Cast<ACelestialBody>(actor);
		if (this->bScaleUpdateRequired)
		{
			body->SetScale(this->m_SizeScale);
			if (actor == this->m_CenterActor)
			{
				// Update the center offset with the new radius of the center
				this->m_CenterOffset = body->GetRadiusWithScale();
			}
		}
		// Center actor does not move so there's no point calling the method
		if (actor != this->m_CenterActor)
		{
			float multiplier = this->m_VelocityMultiplier * delta;
			body->Move(multiplier, this->m_CenterOffset, this->m_DistanceScale);
		}
	}
	if (this->bScaleUpdateRequired)
	{
		this->bScaleUpdateRequired = false;
	}
}

