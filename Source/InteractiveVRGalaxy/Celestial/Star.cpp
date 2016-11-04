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
#define DEFAULT_RADIUS 695700.0f
#define DEFAULT_MASS 1988550.0f
#define DEFAULT_GRAVITY 274.596f

AStar::AStar()
{
	Super::SetMinVelocity(DEFAULT_MIN_SPEED);
	Super::SetMaxVelocity(DEFAULT_MAX_SPEED);
	Super::SetFurthestDistance(DEFAULT_APHELION);
	Super::SetNearestDistance(DEFAULT_PERIHELION);
	Super::SetSemiMajorAxis(DEFAULT_SEMIMAJOR_AXIS);
	Super::SetEccentricity(DEFAULT_ECCENTRICITY);
	Super::SetGravity(DEFAULT_GRAVITY);
	Super::SetRadius(DEFAULT_RADIUS);
	Super::SetMass(DEFAULT_MASS);
}



