// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveVRGalaxy.h"
#include "Orbit.h"

#include "OrbitMeshComponent.h"

#define ORBIT_MATERIAL_COLOR_NAME TEXT("OrbitColor")
#define ORBIT_MATERIAL_LOCATION "Material'/Game/VirtualRealityBP/Materials/Planets/M_Orbit.M_Orbit'"

// Sets default values
AOrbit::AOrbit() : m_Material(nullptr), m_Color(FLinearColor::White), m_Mesh(nullptr),
m_bMeshUpdateRequired(false), m_bMaterialUpdateRequired(false)
{
	this->m_Mesh = UObject::CreateDefaultSubobject<UOrbitMeshComponent>(TEXT("OrbitMesh"));
	Super::RootComponent = this->m_Mesh;

	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT(ORBIT_MATERIAL_LOCATION));
	if (Material.Succeeded())
	{
		this->m_Material = Material.Object;
	}
	Super::PrimaryActorTick.bCanEverTick = true;
}

void AOrbit::Tick(float delta)
{
	this->m_Mesh->SetWorldRotation(FRotator::ZeroRotator);
}

void AOrbit::SetColor(FLinearColor color)
{
	this->m_Color = color;
	this->m_bMaterialUpdateRequired = true;
}

void AOrbit::SetRadius(const float& radius)
{
	this->m_Mesh->SetRadius(radius);
	this->m_bMeshUpdateRequired = true;
}

void AOrbit::SetPoints(TArray<FVector> points)
{
	this->m_Mesh->SetPoints(points);
	this->m_bMeshUpdateRequired = true;
}

void AOrbit::UpdateOrbit()
{
	if (this->m_bMeshUpdateRequired)
	{
		this->m_Mesh->GenerateMesh();

		this->m_bMeshUpdateRequired = false;
		this->m_bMaterialUpdateRequired = true;
	}

	if (this->m_bMaterialUpdateRequired)
	{
		if (this->m_Mesh->GetNumSections() > 0)
		{
			UMaterialInstanceDynamic *material = UMaterialInstanceDynamic::Create(this->m_Material, this->m_Mesh);
			if (material != nullptr)
			{
				material->SetVectorParameterValue(ORBIT_MATERIAL_COLOR_NAME, this->m_Color);
				for (int i = 0; i < this->m_Mesh->GetNumSections(); i++)
				{
					this->m_Mesh->SetMaterial(i, material);
				}
			}
		}
		this->m_bMaterialUpdateRequired = false;
	}
}
