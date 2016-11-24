// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMeshComponent.h"
#include "OrbitMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class INTERACTIVEVRGALAXY_API UOrbitMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()
	
public:
	UOrbitMeshComponent();

	void BeginPlay() override;
	
	void GenerateMesh();

	FORCEINLINE void SetRadius(const float& radius) { this->m_Radius = radius; }

	FORCEINLINE void SetPoints(TArray<FVector> points) { this->m_Points = points; }

private:
	float m_Radius;
	TArray<FVector> m_Points;
};
