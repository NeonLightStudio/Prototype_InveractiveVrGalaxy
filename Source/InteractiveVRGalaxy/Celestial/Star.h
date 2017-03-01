// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Celestial/CelestialBody.h"
#include "Star.generated.h"

/**
 * 
 */
UCLASS()
class INTERACTIVEVRGALAXY_API AStar : public ACelestialBody
{
	GENERATED_BODY()
	
public:
	AStar();

	// Stars only need to rotate so we will provide that functionality
	virtual void Move(const ACelestialBody *center, const float& timeScale, const float& distanceScale, const float& delta) override;

private:
	float m_Timer;
};
