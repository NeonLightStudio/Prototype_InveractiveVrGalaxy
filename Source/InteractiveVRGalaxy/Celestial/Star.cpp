// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveVRGalaxy.h"
#include "Star.h"

//default values are taken from our sun
#define DEFAULT_MIN_SPEED 0.0f
#define DEFAULT_MAX_SPEED 0.0f
#define DEFAULT_APHELION 0.0f
#define DEFAULT_PERIHELION 0.0f
#define DEFAULT_SEMIMAJOR_AXIS 0.0f
#define DEFAULT_ECCENTRICITY 1.0f
#define DEFAULT_ORBIT_PERIOD 0.0f
#define DEFAULT_RADIUS 695700.0f
#define DEFAULT_MASS 1988550.0f
#define DEFAULT_GRAVITY 274.596f
#define DEFAULT_AXIAL_TILT 0.0f
#define DEFAULT_ROTATION_PERIOD TIME_TO_SECONDS(24 DAYS, 11 HOURS, 16 MINUTES, 0 SECONDS)

AStar::AStar()
{
	Super::SetMinVelocity(DEFAULT_MIN_SPEED);
	Super::SetMaxVelocity(DEFAULT_MAX_SPEED);
	Super::SetFurthestDistance(DEFAULT_APHELION);
	Super::SetNearestDistance(DEFAULT_PERIHELION);
	Super::SetSemiMajorAxis(DEFAULT_SEMIMAJOR_AXIS);
	Super::SetEccentricity(DEFAULT_ECCENTRICITY);
	Super::SetOrbitPeriod(DEFAULT_ORBIT_PERIOD);
	Super::SetGravity(DEFAULT_GRAVITY);
	Super::SetRadius(DEFAULT_RADIUS);
	Super::SetMass(DEFAULT_MASS);
	Super::SetAxialTilt(DEFAULT_AXIAL_TILT);
	Super::SetRotationPeriod(DEFAULT_ROTATION_PERIOD);
}

void AStar::Move(const ACelestialBody* center, const float& timeScale, const float& distanceScale, const float& delta)
{
	this->m_Timer += delta * timeScale;

	if(Super::m_RotationScale == 0.0f)
	{
		return;
	}
	Super::m_PlanetRotation = -RAD_TO_DEG * PI2 * this->m_Timer / (Super::GetRotationPeriod() / Super::m_RotationScale);
	Super::m_Root->SetRelativeRotation(FRotator(0.0f, Super::m_PlanetRotation, 0.0f));
}
