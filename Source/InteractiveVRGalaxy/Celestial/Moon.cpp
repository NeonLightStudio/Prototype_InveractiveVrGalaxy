// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveVRGalaxy.h"
#include "Moon.h"

//default values are taken from earths moon
#define DEFAULT_MIN_SPEED 0.964f
#define DEFAULT_MAX_SPEED 1.076f
#define DEFAULT_APOGEE 405400.0f
#define DEFAULT_PERIGEE 362600.0f
#define DEFAULT_SEMIMAJOR_AXIS 384399.0f
#define DEFAULT_ECCENTRICITY 0.0549f
#define DEFAULT_ORBIT_PERIOD 27.321661f
#define DEFAULT_RADIUS 1737.1f
#define DEFAULT_MASS 0.07342f
#define DEFAULT_GRAVITY 1.62f
#define DEFAULT_AXIAL_TILT 1.5424f
#define DEFAULT_ROTATION_PERIOD TIME_TO_SECONDS(0 DAYS, 655.728f HOURS, 0 MINUTES, 0 SECONDS)

AMoon::AMoon()
{
	Super::SetMinVelocity(DEFAULT_MIN_SPEED);
	Super::SetMaxVelocity(DEFAULT_MAX_SPEED);
	Super::SetFurthestDistance(DEFAULT_APOGEE);
	Super::SetNearestDistance(DEFAULT_PERIGEE);
	Super::SetSemiMajorAxis(DEFAULT_SEMIMAJOR_AXIS);
	Super::SetEccentricity(DEFAULT_ECCENTRICITY);
	Super::SetOrbitPeriod(DEFAULT_ORBIT_PERIOD);
	Super::SetGravity(DEFAULT_GRAVITY);
	Super::SetRadius(DEFAULT_RADIUS);
	Super::SetMass(DEFAULT_MASS);
	Super::SetAxialTilt(DEFAULT_AXIAL_TILT);
	Super::SetRotationPeriod(DEFAULT_ROTATION_PERIOD);
	Super::SetRotateOrbitClockwise(false);
	Super::SetRotatePlanetClockwise(true);
}