// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveVRGalaxy.h"
#include "CelestialBody.h"

#define ORBIT_BEAM_SCALE_MULTIPLIER 1.25f
#define GRAVITATIONAL_CONSTANT 6.67408e-11

#define SPHERE_MESH_LOCATION TEXT("StaticMesh'/Game/VirtualRealityBP/Blueprints/Planets/SphereMesh.SphereMesh'")

// Sets default values
ACelestialBody::ACelestialBody() : m_Material(nullptr), m_bDrawOrbit(false), m_OrbitParticleResolution(20), m_OrbitColor(FLinearColor::White),
		m_ParticleSystem(nullptr), m_CurrentSpeed(0.0f), m_Angle(0.0f), m_RotateOrbitClockwise(true)
{
	this->m_Root = UObject::CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyRoot"));
	Super::RootComponent = this->m_Root;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> sphere(SPHERE_MESH_LOCATION);
	if (sphere.Succeeded())
	{
		this->m_Root->SetStaticMesh(sphere.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> particleRef(TEXT("ParticleSystem'/Game/VirtualRealityBP/Materials/Beam/P_Beam.P_Beam'"));
	if(particleRef.Succeeded())
	{
		this->m_ParticleSystem = particleRef.Object;
	}
}

void ACelestialBody::BeginPlay()
{
	this->CalculateSemiMinorAxis();
	this->CalculatePerimeter();

	if (this->m_Material)
	{
		this->m_Root->SetMaterial(0, this->m_Material);
	}
	this->m_Root->SetRelativeRotation(FQuat(FVector(0.0f, 1.0f, 0.0f), -this->m_AxialTilt * DEG_TO_RAD));
}

#if WITH_EDITOR
void ACelestialBody::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName name = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// If the Semi-major Axis or Eccentricity changes we will need to recalculate the Semi-minor axis
	if (name == GET_MEMBER_NAME_CHECKED(ACelestialBody, m_SemiMajorAxis)
		|| name == GET_MEMBER_NAME_CHECKED(ACelestialBody, m_Eccentricity))
	{
		this->CalculateSemiMinorAxis();
		this->CalculatePerimeter();
	}
	if (name == GET_MEMBER_NAME_CHECKED(ACelestialBody, m_Material))
	{
		this->m_Root->SetMaterial(0, this->m_Material);
	}
	if (name == GET_MEMBER_NAME_CHECKED(ACelestialBody, m_AxialTilt))
	{
		this->m_Root->SetRelativeRotation(FQuat(FVector(0.0f, 1.0f, 0.0f), -this->m_AxialTilt * DEG_TO_RAD));
	}
	if (name == GET_MEMBER_NAME_CHECKED(ACelestialBody, m_bDrawOrbit))
	{
		this->m_bDrawOrbit = !this->m_bDrawOrbit;
		this->SetDrawOrbit(!this->m_bDrawOrbit);
	}
	if(this->m_bDrawOrbit && (name == GET_MEMBER_NAME_CHECKED(ACelestialBody, m_OrbitColor)
		|| name == GET_MEMBER_NAME_CHECKED(ACelestialBody, m_OrbitParticleResolution)))
	{
		this->ResetDrawOrbit();
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void ACelestialBody::SetScale(const float& scale) const
{
	check(this->m_Root);
	float newScale = this->m_Radius * scale;
	this->m_Root->SetWorldScale3D(FVector(newScale));
}

float ACelestialBody::CalculateRotation(const float& radians) const
{
	float rotation = FMath::Fmod(this->m_OrbitPeriod * (radians / PI2), 1.0f) * PI2;
	return this->m_RotatePlanetClockwise ? rotation : -rotation;
}

float ACelestialBody::CalculateVelocity(const float& radians) const
{
	return this->m_MinSpeed + (this->m_MaxSpeed - this->m_MinSpeed) * FMath::Sin(radians / 2.0f);
}

float ACelestialBody::CalculateDistance(const float& radians) const
{
	//r = (a(1-e^2))/(1+ecosR)
	return (this->m_SemiMajorAxis * (1.0f - this->m_Eccentricity * this->m_Eccentricity)) / (1.0f + this->m_Eccentricity * FMath::Cos(radians));
}

FVector ACelestialBody::CalculatePosition(const float& radians, const float& offset, const float& distanceScale) const
{
	FVector vector;
	vector.X = (offset + this->m_SemiMajorAxis * distanceScale) * -FMath::Cos(radians);
	vector.Y = (offset + this->m_SemiMinorAxis * distanceScale) * FMath::Sin(radians);
	if (!this->m_RotateOrbitClockwise)
	{
		vector.Y = -vector.Y;
	}
	return Super::GetAttachParentActor()->GetActorLocation() + vector;
}

void ACelestialBody::ResetDrawOrbit()
{
	if(!this->m_bDrawOrbit)
	{
		return;
	}
	this->SetDrawOrbit(false);
	this->SetDrawOrbit(true);
}

void ACelestialBody::SetDrawOrbit(const bool& draw)
{
	if(this->m_bDrawOrbit == draw || (draw && this->m_ParticleSystem == nullptr))
	{
		return;
	}
	this->m_bDrawOrbit = draw;
	if(!draw)
	{
		for(int i = 0; i < this->m_OrbitParticleSystems.Num(); i++)
		{
			this->m_OrbitParticleSystems[i]->DestroyComponent();
		}
		this->m_OrbitParticleSystems.Empty();
	}
	else
	{
		check(this->m_OrbitParticleResolution >= 0);

		FVector prevLoc = this->m_Root->GetComponentLocation();
		for(int i = 0; i < this->m_OrbitParticleResolution; i++)
		{
			UParticleSystemComponent *system = UGameplayStatics::SpawnEmitterAtLocation(Super::GetWorld(),
				this->m_ParticleSystem, this->m_Root->GetComponentLocation(), FRotator::ZeroRotator, false);
			if(system == nullptr)
			{
				continue;
			}
			float currentRad = PI2 * (i + 1) / this->m_OrbitParticleResolution;
			FVector target = this->CalculatePosition(currentRad, this->m_LastOffset, this->m_LastDistanceScale);
			system->SetBeamSourcePoint(0, prevLoc, 0);
			system->SetBeamTargetPoint(0, prevLoc = target, 0);

			system->SetColorParameter(FName("InitialColor"), this->m_OrbitColor);
			// Set the scale of the beam to be equal to the bodies scale. Has an additional multiplier to increase or decrease the scale using the bodies as a base.
			system->SetWorldScale3D(FVector(ORBIT_BEAM_SCALE_MULTIPLIER * this->GetRadiusWithScale() * 2.0f));

			this->m_OrbitParticleSystems.Add(system);
		}
	}
}

void ACelestialBody::Move(const ACelestialBody *center, const float& multiplier, const float& distanceScale)
{
	check(this->m_Root);
	float velocity = this->CalculateVelocity(this->GetAngleToCenter() * DEG_TO_RAD);
	this->m_CurrentSpeed = velocity;

	float kmPerDegree = this->m_Perimeter * distanceScale / 360.0f;
	check(kmPerDegree != 0.0f);

	float offset;
	if (Super::GetAttachParentActor()->IsA(ACelestialBody::StaticClass()))
	{
		offset = ((ACelestialBody*)Super::GetAttachParentActor())->GetRadiusWithScale();
	}
	else
	{
		offset = center->GetRadiusWithScale();
	}
	this->m_LastOffset = offset;
	this->m_LastDistanceScale = distanceScale;

	this->m_Angle += (velocity * distanceScale * multiplier / kmPerDegree);
	this->m_Angle = FMath::Fmod(this->m_Angle, 360.0f);
	this->m_Root->SetWorldLocation(this->CalculatePosition(this->m_Angle * DEG_TO_RAD, offset, distanceScale));
	//this->m_Root->SetRelativeLocation(this->CalculatePosition(this->m_Angle * DEG_TO_RAD, distanceOffset, distanceScale));

	FRotator rotator(0.0f, this->CalculateRotation(this->m_Angle * DEG_TO_RAD) * RAD_TO_DEG, 0.0f);
	this->m_PlanetRotation = rotator.Yaw;
	this->m_Root->SetRelativeRotation(rotator);
}