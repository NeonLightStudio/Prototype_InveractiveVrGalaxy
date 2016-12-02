// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveVRGalaxy.h"
#include "DomeMeshComponent.h"

#define DOME_COLLISION false

#define DOME_VERTICAL_SEGMENTS 30	//default
#define DOME_HORIZONTAL_SEGMENTS 20 //default

UDomeMeshComponent::UDomeMeshComponent()
{
	// Set default vertical segments
	this->m_VerticalSegments = DOME_VERTICAL_SEGMENTS;

	// Set default horizontal segments
	this->m_HorizontalSegments = DOME_HORIZONTAL_SEGMENTS;

	// Set to only generate half a sphere
	this->m_bFullSphere = false;

	//this->CreateDome(); // Can't create mesh here. It must be done after the initialization of the instance (BeginPlay)
}

void UDomeMeshComponent::BeginPlay()
{
	this->CreateDome();
}

void UDomeMeshComponent::CreateDome()
{
	Super::ClearAllMeshSections();

	FVector temp;
	int32 count = 0;
	const TArray<int32> indices{ 0, 1, 2, 2, 3, 0 };
	const float theta = PI2 / this->m_VerticalSegments;
	const float delta = PI / this->m_HorizontalSegments;
	for (float longitude = 0.0f; longitude < PI2 - theta; longitude += theta)
	{
		float latMax = this->m_bFullSphere ? PI : PI / 2.0f;
		for (float latitude = 0.0f; latitude < latMax; latitude += delta)
		{
			TArray<FVector> vertices, normals;
			TArray<FVector2D> uv;

			temp.X = FMath::Cos(longitude + theta) * FMath::Sin(latitude + delta);
			temp.Y = FMath::Cos(latitude + delta);
			temp.Z = FMath::Sin(longitude + theta) * FMath::Sin(latitude + delta);
			vertices.Add(temp); 
			uv.Add(FVector2D(0.0f, 1.0f));
			//uv.Add(FVector2D(1.0f - (longitude + theta) / PI2 - 0.25f, (latitude + delta) / PI));

			temp.X = FMath::Cos(longitude) * FMath::Sin(latitude + delta);
			temp.Y = FMath::Cos(latitude + delta);
			temp.Z = FMath::Sin(longitude) * FMath::Sin(latitude + delta);
			vertices.Add(temp); 
			uv.Add(FVector2D(1.0f, 1.0f));
			//uv.Add(FVector2D(1.0f - longitude / PI2 - 0.25f, (latitude + delta - PI / 180.0f) / PI));

			temp.X = FMath::Cos(longitude) * FMath::Sin(latitude);
			temp.Y = FMath::Cos(latitude);
			temp.Z = FMath::Sin(longitude) * FMath::Sin(latitude);
			vertices.Add(temp); 
			uv.Add(FVector2D(1.0f, 0.0f));
			//uv.Add(FVector2D(1.0f - longitude / PI2 - 0.25f, latitude / PI));

			temp.X = FMath::Cos(longitude + theta) * FMath::Sin(latitude);
			temp.Y = FMath::Cos(latitude);
			temp.Z = FMath::Sin(longitude + theta) * FMath::Sin(latitude);
			vertices.Add(temp); 
			uv.Add(FVector2D(0.0f, 0.0f));
			//uv.Add(FVector2D(1.0f - (longitude + theta) / PI2 - 0.25f, latitude / PI));

			// Calculate average normal between the 4 vertices
			FVector normal(0.0f);
			for (int i = 0; i < vertices.Num(); i++)
			{
				normal += vertices[i];
			}
			normal /= vertices.Num();
			normals.Init(normal, vertices.Num());
			
			// Create mesh section. Each section reprsents one panel. (one square)
			Super::CreateMeshSection(count++, vertices, indices, normals, uv, TArray<FColor>(), TArray<FProcMeshTangent>(), DOME_COLLISION);
		}
	}
	//UE_LOG(LogClass, Log, TEXT("Section Counter: %d"), Super::GetNumSections());
}