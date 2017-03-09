// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveVRGalaxy.h"
#include "CelestialBody.h"

#define ORBIT_BEAM_SCALE_MULTIPLIER 1.25f
#define GRAVITATIONAL_CONSTANT 6.67408e-11

#define SPHERE_MESH_LOCATION TEXT("StaticMesh'/Game/VirtualRealityBP/Blueprints/Planets/SphereMesh.SphereMesh'")

// Sets default values
ACelestialBody::ACelestialBody() : m_Mesh(nullptr), m_Material(nullptr), m_MaterialDynamic(nullptr), m_Atmosphere(nullptr), m_Orbit(nullptr)
{
	// Atmosphere
	this->m_bDrawAtmosphere = false;

	// Draw Orbit
	this->m_bDrawOrbit = false;
	this->m_DrawOrbitRadius = 40.0f;
	this->m_DrawOrbitResolution = 50;
	this->m_DrawOrbitColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.25f);

	// Orbit
	this->m_CurrentSpeed = 0.0f;
	this->m_Angle = 0.0f;
	this->m_RotateOrbitClockwise = true;

	// Scale
	this->m_VelocityScale = 1.0f;
	this->m_RotationScale = 1.0f;
	this->m_RadiusScale = 1.0f;
	this->m_OrbitDistanceScale = 1.0f;

	// Movement
	this->m_bMoveBody = true;
	this->m_bMoveBodyTransition = false;
	this->m_TransitionDelay = 1.0f;

	// World
	this->m_SunLocation = FVector(0.0f);

	// Other
	this->m_Root = UObject::CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyRoot"));
	if (this->m_Material)
	{
		this->m_Root->SetMaterial(0, this->m_Material);
	}
	Super::RootComponent = this->m_Root;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> sphere(SPHERE_MESH_LOCATION);
	if (sphere.Succeeded())
	{
		this->m_Root->SetStaticMesh(this->m_Mesh = sphere.Object);
	}
}

void ACelestialBody::BeginPlay()
{
	this->CalculateSemiMinorAxis();
	this->CalculatePerimeter();

	// Check to see if this planet has been spawned without a solar system
	if (Super::GetAttachParentActor() == nullptr)
	{
		this->SetScale(1.0f);
	}
	this->m_MaterialDynamic = UMaterialInstanceDynamic::Create(this->m_Material, this->m_Root);

	this->SetSunLocation(this->m_SunLocation);
	if (this->m_bDrawOrbit)
	{
		this->m_bDrawOrbit = false;
		this->SetDrawOrbit(true);
	}
	if (this->m_bDrawAtmosphere)
	{
		this->m_bDrawAtmosphere = false;
		this->SetDrawAtmosphere(true);
	}
	this->m_Root->SetRelativeRotation(FQuat(FVector(0.0f, 1.0f, 0.0f), -this->m_AxialTilt * DEG_TO_RAD));
}

void ACelestialBody::SetSunLocation(const FVector& location)
{
	this->m_SunLocation = location;

	if (this->m_MaterialDynamic)
	{
		this->m_MaterialDynamic->SetVectorParameterValue(MATERIAL_NAME_SUN_LOCATION, this->m_SunLocation);
		this->m_Root->SetMaterial(0, this->m_MaterialDynamic);
	}
	if (this->m_Atmosphere != nullptr)
	{
		this->m_Atmosphere->UpdateAtmosphere();
	}
}

void ACelestialBody::SetScale(const float& scale)
{
	check(this->m_Root);
	float newScale = this->m_Radius * scale * this->m_RadiusScale;
	this->m_Root->SetWorldScale3D(FVector(newScale));
	this->m_LastRadiusScale = scale;
	if (this->m_Atmosphere != nullptr)
	{
		this->m_Atmosphere->UpdateAtmosphere();
	}
	if (this->m_bDrawOrbit)
	{
		this->ResetDrawOrbit();
	}
}

float ACelestialBody::CalculateRotation(const float& radians) const
{
	float rotation = FMath::Fmod(this->m_OrbitPeriod * this->m_RotationScale * (radians / PI2), 1.0f) * PI2;
	return this->m_RotatePlanetClockwise ? rotation : -rotation;
}

float ACelestialBody::CalculateVelocity(const float& radians) const
{
	return (this->m_MinSpeed + (this->m_MaxSpeed - this->m_MinSpeed) * FMath::Sin(radians / 2.0f)) * this->m_VelocityScale;
}

float ACelestialBody::CalculateDistance(const float& radians) const
{
	//r = (a(1-e^2))/(1+ecosR)
	return (this->m_SemiMajorAxis * (1.0f - this->m_Eccentricity * this->m_Eccentricity)) / (1.0f + this->m_Eccentricity * FMath::Cos(radians));
}

FVector ACelestialBody::CalculatePosition(const float& radians, const float& offset, const float& distanceScale) const
{
	FVector vector;
	vector.X = (offset + this->m_SemiMajorAxis * distanceScale * this->m_OrbitDistanceScale) * -FMath::Cos(radians);
	vector.Y = (offset + this->m_SemiMinorAxis * distanceScale * this->m_OrbitDistanceScale) * FMath::Sin(radians);
	vector.Z = 0.0f;
	if (!this->m_RotateOrbitClockwise)
	{
		vector.Y = -vector.Y;
	}
	return Super::GetAttachParentActor()->GetActorLocation() + vector;
}

void ACelestialBody::ResetDrawOrbit()
{
	if (this->m_bDrawOrbit)
	{
		this->SetDrawOrbit(false); //destroy the current orbit actor
		this->SetDrawOrbit(true);  //create a new orbit actor
	}
}

void ACelestialBody::SetDrawOrbit(const bool& draw)
{
	if (this->m_bDrawOrbit == draw)
	{
		return;
	}
	this->m_bDrawOrbit = draw;
	if (Super::GetWorld() == nullptr)
	{
		return;
	}
	if (!draw)
	{
		// If we no longer want to see the orbit, destroy the actor
		if (this->m_Orbit != nullptr)
		{
			this->m_Orbit->Destroy();
			this->m_Orbit = nullptr;
		}
	}
	else
	{
		// Check to see if we have a parent actor to attach the orbit to (we can't attach to ourselves due to relative positioning)
		AActor *parent = Super::GetAttachParentActor();
		if (parent == nullptr)
		{
			// If we have no parent, we cannot create the orbit actor
			this->m_bDrawOrbit = false;
			return;
		}
		// Create the orbit actor
		this->m_Orbit = (AOrbit*)Super::GetWorld()->SpawnActor(AOrbit::StaticClass());
		if (this->m_Orbit)
		{
			// Set the points of the orbit accoding to the preset resolution
			TArray<FVector> points;
			for (int i = 0; i < this->m_DrawOrbitResolution; i++)
			{
				float delta = PI2 * (i + 1) / this->m_DrawOrbitResolution;
				points.Add(this->CalculatePosition(delta, this->m_LastOffset, this->m_LastDistanceScale) - parent->GetActorLocation());
			}
			this->m_Orbit->SetPoints(points);
			this->m_Orbit->SetColor(this->m_DrawOrbitColor);
			this->m_Orbit->SetRadius(this->m_DrawOrbitRadius);
			this->m_Orbit->AttachToActor(parent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			// Update the material parameters to match what we just sent in
			this->m_Orbit->UpdateOrbit();
		}
	}
}

void ACelestialBody::SetDrawAtmosphere(const bool& draw)
{
	if (this->m_bDrawAtmosphere == draw)
	{
		return;
	}
	this->m_bDrawAtmosphere = draw;
	if (Super::GetWorld() == nullptr)
	{
		return;
	}
	if (!draw)
	{
		// If we don't want the atmosphere anymore, destroy it
		if (this->m_Atmosphere != nullptr)
		{
			this->m_Atmosphere->Destroy();
			this->m_Atmosphere = nullptr;
		}
	}
	else
	{
		// If we want an atmosphere, create it and then set the attributes to be equal to what is set within this class
		this->m_Atmosphere = (AAtmosphere*)Super::GetWorld()->SpawnActor(AAtmosphere::StaticClass());
		if (this->m_Atmosphere)
		{
			this->m_Atmosphere->SetAtmosphereData(&this->m_AtmosphereData);
			this->m_Atmosphere->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			this->m_Atmosphere->UpdateAtmosphere();
		}
	}
}

void ACelestialBody::Move(const ACelestialBody *center, const float& timeScale, const float& distanceScale, const float& delta)
{
	float velocity = this->CalculateVelocity(this->GetAngleToCenter() * DEG_TO_RAD);
	this->m_CurrentSpeed = velocity;

	float kmPerDegree = this->m_Perimeter * distanceScale * this->m_OrbitDistanceScale / 360.0f;
	if(kmPerDegree != 0.0f)
	{
		// Check to see whether we want to offset from the center or the parent actor
		float offset;
		if (Super::GetAttachParentActor()->IsA(ACelestialBody::StaticClass()))
		{
			offset = ((ACelestialBody*)Super::GetAttachParentActor())->GetRadiusWithScale();
		}
		else
		{
			offset = center->GetRadiusWithScale();
		}
		// Check to see if the calling actor has changed the offset or distance scale
		// If we have an orbit actor on this body then we will need to reset it to keep it accurate
		bool resetOrbit = this->m_bDrawOrbit && (!FMath::IsNearlyEqual(this->m_LastOffset, offset)
			|| !FMath::IsNearlyEqual(this->m_LastDistanceScale, distanceScale));
		this->m_LastOffset = offset;
		this->m_LastDistanceScale = distanceScale;
		if (resetOrbit)
		{
			UE_LOG(LogClass, Log, TEXT("1"));
			// Recreate the orbit with the new offset and scale parameters in mind
			this->ResetDrawOrbit();
		}

		this->m_Angle += (velocity * distanceScale * this->m_OrbitDistanceScale * timeScale * delta / kmPerDegree);
		this->m_Angle = FMath::Fmod(this->m_Angle, 360.0f);
	}

	this->m_PlanetRotation = this->CalculateRotation(this->m_Angle * DEG_TO_RAD) * RAD_TO_DEG;

	// If we want this body to physically move in the world
	if (this->m_bMoveBody)
	{
		FVector targetPosition = this->CalculatePosition(this->m_Angle * DEG_TO_RAD, this->m_LastOffset, distanceScale);
		FRotator targetRotation = FRotator(0.0f, this->m_PlanetRotation, 0.0f);
		// Check to see if we need to transition back into the orbit, or if we are already in the orbit
		if (this->m_bMoveBodyTransition)
		{
			// If we need to transition, linear interpolate from the current position back to the orbit position over a delay
			this->m_TransitionTimer += delta;
			if (this->m_TransitionTimer >= this->m_TransitionDelay)
			{
				this->m_bMoveBodyTransition = false;
				this->m_TransitionTimer = this->m_TransitionDelay;
			}
			float perc = FMath::Sin(this->m_TransitionTimer / this->m_TransitionDelay * PI / 2.0f); // use sine for a smoother transition
			this->m_Root->SetWorldLocation(FMath::Lerp(this->m_TransitionPosStart, targetPosition, perc));
			this->m_Root->SetRelativeRotation(FMath::Lerp(this->m_TransitionRotStart, targetRotation, perc));
		}
		else
		{
			this->m_Root->SetRelativeRotation(targetRotation);
			this->m_Root->SetWorldLocation(targetPosition);
		}
	}
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
		if (this->m_bDrawOrbit)
		{
			this->ResetDrawOrbit();
		}
	}
	if (name == GET_MEMBER_NAME_CHECKED(ACelestialBody, m_Material))
	{
		this->m_MaterialDynamic = UMaterialInstanceDynamic::Create(this->m_Material, this->m_Root);
		// Update material parameters
		this->SetSunLocation(this->m_SunLocation);
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
	if (name == GET_MEMBER_NAME_CHECKED(ACelestialBody, m_bDrawAtmosphere))
	{
		this->m_bDrawAtmosphere = !this->m_bDrawAtmosphere;
		this->SetDrawAtmosphere(!this->m_bDrawAtmosphere);
	}
	if (name == GET_MEMBER_NAME_CHECKED(ACelestialBody, m_RadiusScale))
	{
		this->SetRadiusScale(this->m_RadiusScale);
	}
	if (name == GET_MEMBER_NAME_CHECKED(ACelestialBody, m_DrawOrbitRadius) && this->m_Orbit != nullptr)
	{
		this->m_Orbit->SetRadius(this->m_DrawOrbitRadius);
		this->m_Orbit->UpdateOrbit();
	}
	if (name == GET_MEMBER_NAME_CHECKED(ACelestialBody, m_DrawOrbitColor) && this->m_Orbit != nullptr)
	{
		this->m_Orbit->SetColor(this->m_DrawOrbitColor);
		this->m_Orbit->UpdateOrbit();
	}
	if (name == GET_MEMBER_NAME_CHECKED(ACelestialBody, m_DrawOrbitResolution) && this->m_Orbit != nullptr)
	{
		this->SetDrawOrbit(false);
		this->SetDrawOrbit(true);
	}
	if (name == GET_MEMBER_NAME_CHECKED(ACelestialBody, m_bMoveBody))
	{
		this->SetMoveBody(this->m_bMoveBody);
	}

	if (name == GET_MEMBER_NAME_CHECKED(ACelestialBody, m_SunLocation))
	{
		this->SetSunLocation(this->m_SunLocation);
	}

	if (name == GET_MEMBER_NAME_CHECKED(ACelestialBody, m_Mesh))
	{
		this->m_Root->SetStaticMesh(this->m_Mesh);
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif