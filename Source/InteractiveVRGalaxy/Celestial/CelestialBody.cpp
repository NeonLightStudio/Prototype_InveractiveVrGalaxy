// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveVRGalaxy.h"
#include "CelestialBody.h"

#define GRAVITATIONAL_CONSTANT 6.67408e-11

#define SPHERE_MESH_LOCATION TEXT("StaticMesh'/Game/VirtualRealityBP/Blueprints/Planets/SphereMesh.SphereMesh'")

// Sets default values
ACelestialBody::ACelestialBody() : m_Angle(0.0f), m_CurrentSpeed(0.0f), m_Material(nullptr)
{
	this->m_Root = UObject::CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyRoot"));
	Super::RootComponent = this->m_Root;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> sphere(SPHERE_MESH_LOCATION);
	if (sphere.Succeeded())
	{
		this->m_Root->SetStaticMesh(sphere.Object);
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
	FName name = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

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
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void ACelestialBody::SetScale(const float& size)
{
	check(this->m_Root);
	float newScale = this->m_Radius * size;
	this->m_Root->SetWorldScale3D(FVector(newScale));
}

float ACelestialBody::CalculateRotation(const float& radians) const
{
	return FMath::Fmod(this->m_OrbitPeriod * (radians / PI2), 1.0f) * PI2;
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

FVector ACelestialBody::CalculatePosition(const float& radians, const float& distanceOffset, const float& distanceScale) const
{
	FVector vector;
	vector.X = (distanceOffset + this->m_SemiMajorAxis * distanceScale) * -FMath::Cos(radians);
	vector.Y = (distanceOffset + this->m_SemiMinorAxis * distanceScale) * FMath::Sin(radians);
	return vector;
}

void ACelestialBody::Move(const float& multiplier, const float& distanceOffset, const float& distanceScale)
{
	check(this->m_Root);
	float velocity = this->CalculateVelocity(this->GetAngleToCenter() * DEG_TO_RAD);
	this->m_CurrentSpeed = velocity;

	float kmPerDegree = this->m_Perimeter * distanceScale / 360.0f;
	check(kmPerDegree != 0.0f);

	this->m_Angle += (velocity * distanceScale * multiplier / kmPerDegree);
	this->m_Angle = FMath::Fmod(this->m_Angle, 360.0f);
	this->m_Root->SetRelativeLocation(this->CalculatePosition(this->m_Angle * DEG_TO_RAD, distanceOffset, distanceScale));

	FRotator rotator(0.0f, this->CalculateRotation(this->m_Angle * DEG_TO_RAD) * RAD_TO_DEG, 0.0f);
	this->m_PlanetRotation = rotator.Yaw;
	this->m_Root->SetRelativeRotation(rotator);
}