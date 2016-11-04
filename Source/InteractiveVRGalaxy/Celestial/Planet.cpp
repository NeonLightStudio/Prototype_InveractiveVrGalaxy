// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveVRGalaxy.h"
#include "Planet.h"

//default values are taken from earth
#define DEFAULT_MIN_SPEED 29.29f
#define DEFAULT_MAX_SPEED 30.29f
#define DEFAULT_APHELION 152100643.1f
#define DEFAULT_PERIHELION 147094903.846f
#define DEFAULT_SEMIMAJOR_AXIS 149598023.f
#define DEFAULT_ECCENTRICITY 0.0167086f
#define DEFAULT_ORBIT_PERIOD 365.256363004f
#define DEFAULT_RADIUS 6371.0f
#define DEFAULT_MASS 5.97237f
#define DEFAULT_GRAVITY 9.807f
#define DEFAULT_AXIAL_TILT 23.4392811f
#define DEFAULT_ROTATION_PERIOD TIME_TO_SECONDS(0 DAYS, 23 HOURS, 56 MINUTES, 4.1f SECONDS)  //86164.1f

APlanet::APlanet()
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

