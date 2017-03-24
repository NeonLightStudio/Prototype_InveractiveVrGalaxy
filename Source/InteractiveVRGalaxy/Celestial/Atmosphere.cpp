// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveVRGalaxy.h"

#include "CelestialBody.h"
#include "Atmosphere.h"

#define MATERIAL_NAME_ATMOSPHERE_COLOR FName("AtmosphereColor")

#define SPHERE_MESH_LOCATION TEXT("StaticMesh'/Game/VirtualRealityBP/Blueprints/Planets/SphereMesh.SphereMesh'")
#define ATMOSPHERE_MATERIAL_LOCATION TEXT("Material'/Game/VirtualRealityBP/Materials/Planets/M_Atmosphere.M_Atmosphere'")

// Sets default values
AAtmosphere::AAtmosphere() : m_Root(nullptr), m_Data(nullptr)
{
	this->m_Root = UObject::CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AtmosphereRoot"));
	Super::RootComponent = this->m_Root;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(SPHERE_MESH_LOCATION);
	if (mesh.Succeeded())
	{
		this->m_Root->SetStaticMesh(mesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> material(ATMOSPHERE_MATERIAL_LOCATION);
	if (material.Succeeded())
	{
		this->m_Root->SetMaterial(0, material.Object);
	}
}

void AAtmosphere::BeginPlay()
{
	this->m_Material = UMaterialInstanceDynamic::Create(this->m_Root->GetMaterial(0), this->m_Root);

	this->UpdateAtmosphere();
}

void AAtmosphere::UpdateAtmosphere() const
{
	check(this->m_Root);
	if (this->m_Data == nullptr || this->m_Material == nullptr)
	{
		return;
	}
	AActor *parent = Super::GetAttachParentActor();
	if (parent != nullptr && parent->IsA(ACelestialBody::StaticClass()))
	{
		ACelestialBody *body = (ACelestialBody*)parent;

		// Update sun location
		this->m_Material->SetVectorParameterValue(MATERIAL_NAME_SUN_LOCATION, body->GetSunLocation());

		//UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(this->m_Root->GetMaterial(0), this->m_Root); 
		this->m_Material->SetVectorParameterValue(MATERIAL_NAME_ATMOSPHERE_COLOR, this->m_Data->m_Color);
		this->m_Root->SetMaterial(0, this->m_Material);

		this->m_Root->SetWorldScale3D(FVector((body->GetRadius() + this->m_Data->m_Height
			* this->m_Data->m_HeightScale) * body->GetSolarSystemRadiusScale() * body->GetRadiusScale()));
	}
}

