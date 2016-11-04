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
	this->SpawnBodies(this, this->m_CelestialBodies);

	AActor *center = Super::GetWorld()->SpawnActor(this->m_Center);
	if (center)
	{
		this->m_CenterActor = (ACelestialBody*)center;
		center->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);

		this->m_AttachedBodies.Add(this->m_CenterActor);
	}
	this->bScaleUpdateRequired = true;

	//for (float i = 0; i < PI2; i += PI2 / 20)
	//{
	//	float x = FMath::Cos(i), y = FMath::Sin(i);
	//	float r = ((ACelestialBody*)m_AttachedActors[1])->CalculateDistance(i);
	//	UE_LOG(LogClass, Log, TEXT("%f %f"), r, ((ACelestialBody*)m_AttachedActors[1])->CalculateVelocity(i));
	//	//DrawDebugSphere(Super::GetWorld(), FVector(x * r * this->m_DistanceScale, y * r * this->m_DistanceScale, 0.0f), 50, 20, FColor::Red, true);
	//}

}

void ASolarSystem::SpawnBodies(AActor *parent, const TArray<TSubclassOf<ACelestialBody>>& bodies)
{
	check(parent);
	// Add all the bodies in reverse order since AttachToActor works using a push-to-front mechanic
	for (int i = bodies.Num() - 1; i >= 0; i--)
	{
		ACelestialBody *actor = (ACelestialBody*)Super::GetWorld()->SpawnActor(bodies[i]);
		if (actor == nullptr)
		{
			continue;
		}
		actor->AttachToActor(parent, FAttachmentTransformRules::SnapToTargetIncludingScale);
		this->m_AttachedBodies.Add(actor);

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

	for (AActor *actor : this->m_AttachedBodies)
	{
		if (!actor->IsA(ACelestialBody::StaticClass()))
		{
			continue;
		}
		ACelestialBody *body = Cast<ACelestialBody>(actor);
		if (this->bScaleUpdateRequired)
		{
			body->SetScale(this->m_SizeScale);
			if (body == this->m_CenterActor)
			{
				this->m_CenterOffset = body->GetRadiusWithScale();
			}
		}
		// Center actor does not move so there's no point calling the method
		if (actor != this->m_CenterActor)
		{
			float multiplier = this->m_VelocityMultiplier * delta;
			body->Move(this->m_CenterActor, multiplier, this->m_DistanceScale);
		}
	}
	if (this->bScaleUpdateRequired)
	{
		this->bScaleUpdateRequired = false;
	}
}

