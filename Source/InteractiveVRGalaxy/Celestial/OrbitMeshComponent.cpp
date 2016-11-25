// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveVRGalaxy.h"
#include "OrbitMeshComponent.h"

#define CYLINDER_POINTS 12
#define CYLINDER_RADIUS 10.0f

UOrbitMeshComponent::UOrbitMeshComponent() : m_Radius(CYLINDER_RADIUS)
{
}

void UOrbitMeshComponent::BeginPlay()
{
	this->GenerateMesh();
}

void UOrbitMeshComponent::GenerateMesh()
{
	Super::ClearAllMeshSections();
	if(this->m_Points.Num() < 3 || this->m_Radius <= 0.0f)
	{
		// There's not enough sample points to make a circle or the radius is invalid
		return;
	}
	for(int i = 1; i <= this->m_Points.Num(); i++)
	{
		int currIndex = i == this->m_Points.Num() ? 0 : i; //wrap around to start of loop if necessary
		int prevIndex = i - 1; //should never be out-of-bounds if we start at index 1

		TArray<FVector> vertices;
		TArray<int32> indices;// { 0, 1, 2, 2, 3, 0 };

		for(int j = 1; j <= CYLINDER_POINTS; j++)
		{
			int currP = j == CYLINDER_POINTS ? 0 : j;
			int prevP = j - 1;

			float theta = currP * PI2 / CYLINDER_POINTS, delta = prevP * PI2 / CYLINDER_POINTS;
			vertices.Add({ this->m_Points[currIndex] + FVector(FMath::Cos(theta) * this->m_Radius, 0.0f, FMath::Sin(theta) * this->m_Radius) });
			vertices.Add({ this->m_Points[currIndex] + FVector(FMath::Cos(delta) * this->m_Radius, 0.0f, FMath::Sin(delta) * this->m_Radius) });
			vertices.Add({ this->m_Points[prevIndex] + FVector(FMath::Cos(delta) * this->m_Radius, 0.0f, FMath::Sin(delta) * this->m_Radius) });
			vertices.Add({ this->m_Points[prevIndex] + FVector(FMath::Cos(theta) * this->m_Radius, 0.0f, FMath::Sin(theta) * this->m_Radius) });

			for(auto idx : {0, 1, 2, 2, 3, 0})
			{
				indices.Add(idx + (j - 1) * 4);
			}
		}
		Super::CreateMeshSection(i - 1, vertices, indices, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	}
}
