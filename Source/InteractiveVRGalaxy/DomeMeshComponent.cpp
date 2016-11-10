// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveVRGalaxy.h"
#include "DomeMeshComponent.h"

#define DOME_VERTICAL_SEGMENTS 30	//default
#define DOME_HORIZONTAL_SEGMENTS 20 //default

#define DOME_MATERIAL_OPAQUE_LOCATION "Material'/Game/VirtualRealityBP/Materials/M_DomeMaterial_Opaque.M_DomeMaterial_Opaque'"
#define DOME_MATERIAL_TRANSPARENT_LOCATION "Material'/Game/VirtualRealityBP/Materials/M_DomeMaterial_Transparent.M_DomeMaterial_Transparent'"

UDomeMeshComponent::UDomeMeshComponent()
{
	// set default vertical segments
	this->m_VerticalSegments = DOME_VERTICAL_SEGMENTS;

	// set default horizontal segments
	this->m_HorizontalSegments = DOME_HORIZONTAL_SEGMENTS;

	static ConstructorHelpers::FObjectFinder<UMaterial> opaque(TEXT(DOME_MATERIAL_OPAQUE_LOCATION));
	check(opaque.Succeeded());
	this->m_MaterialOpaque = opaque.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial> transparent(TEXT(DOME_MATERIAL_TRANSPARENT_LOCATION));
	check(transparent.Succeeded());
	this->m_MaterialTransparent = transparent.Object;

	//ConstructorHelpers::FObjectFinder<UMaterial> s_Material(TEXT("Material'/Game/VirtualRealityBP/Materials/M_DomeMaterial_Transparent.M_DomeMaterial_Transparent'"));
	//this->m_Material = s_Material.Object;
	// set the material if it is defined
	//Super::SetMaterial(0, this->m_Material);
	
	//this->CreateDome(); // Can't create mesh here. It must be done after the initialization of the instance (BeginPlay)
}

void UDomeMeshComponent::BeginPlay()
{
	this->CreateDome();
	this->SetOpaque();
}

void UDomeMeshComponent::SetOpaque()
{
	if (Super::GetNumSections() == 0)
	{
		return;
	}
	check(this->m_MaterialOpaque);
	UMaterialInstanceDynamic *material = UMaterialInstanceDynamic::Create(this->m_MaterialOpaque, this);
	for (int i = 0; i < Super::GetNumSections(); i++)
	{
		Super::SetMaterial(i, material);
	}
}

void UDomeMeshComponent::SetTransparent()
{
	if (Super::GetNumSections() == 0)
	{
		return;
	}
	check(this->m_MaterialTransparent);
	for (int i = 0; i < Super::GetNumSections(); i++)
	{
		UMaterialInstanceDynamic *material = UMaterialInstanceDynamic::Create(this->m_MaterialTransparent, this);
		material->SetScalarParameterValue(FName("BlendDuration"), FMath::RandRange(0.4f, 2.0f));
		material->SetScalarParameterValue(FName("StartTime"), UGameplayStatics::GetRealTimeSeconds(this));
		material->SetScalarParameterValue(FName("TimeOffset"), FMath::RandRange(0.0f, 0.8f));
		//material->SetScalarParameterValue(FName("TimeOffset"), UGameplayStatics::GetRealTimeSeconds(this) + (i / (float)Super::GetNumSections()) * 2);//+ FMath::RandRange(0.0f, 2.0f));
		Super::SetMaterial(i, material);
	}
}

/*
#if WITH_EDITOR
void UDomeMeshComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName name = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (name == GET_MEMBER_NAME_CHECKED(UDomeMeshComponent, m_VerticalSegments)
		|| name == GET_MEMBER_NAME_CHECKED(UDomeMeshComponent, m_HorizontalSegments))
	{
		this->CreateDome();
	}
}
#endif
*/

void UDomeMeshComponent::CreateDome()
{
	Super::ClearAllMeshSections();

	

	int32 count = 0;
	FVector temp;
	TArray<int32> indices{ 0, 1, 2, 2, 3, 0 };
	const float theta = PI2 / this->m_VerticalSegments;
	const float delta = PI / this->m_HorizontalSegments;
	for (float longitude = theta; longitude < PI2; longitude += theta)
	{
		for (float latitude = 0.0f; latitude < PI / 2.0f; latitude += delta)
		{
			TArray<FVector> vertices, normals;
			TArray<FVector2D> uv;

			temp.X = FMath::Cos(longitude + theta) * FMath::Sin(latitude + delta);
			temp.Y = FMath::Cos(latitude + delta);
			temp.Z = FMath::Sin(longitude + theta) * FMath::Sin(latitude + delta);
			vertices.Add(temp); //normals.Add(temp);
			uv.Add(FVector2D(0.0f, 1.0f));
			//uv.Add(FVector2D(1.0f - (longitude + theta) / PI2 - 0.25f, (latitude + delta) / PI));

			for (int i = 0; i < 4; i++)
			{
				normals.Add(temp);
			}

			temp.X = FMath::Cos(longitude) * FMath::Sin(latitude + delta);
			temp.Y = FMath::Cos(latitude + delta);
			temp.Z = FMath::Sin(longitude) * FMath::Sin(latitude + delta);
			vertices.Add(temp); //normals.Add(temp);
			uv.Add(FVector2D(1.0f, 1.0f));
			//uv.Add(FVector2D(1.0f - longitude / PI2 - 0.25f, (latitude + delta - PI / 180.0f) / PI));

			temp.X = FMath::Cos(longitude) * FMath::Sin(latitude);
			temp.Y = FMath::Cos(latitude);
			temp.Z = FMath::Sin(longitude) * FMath::Sin(latitude);
			vertices.Add(temp); //normals.Add(temp);
			uv.Add(FVector2D(1.0f, 0.0f));
			//uv.Add(FVector2D(1.0f - longitude / PI2 - 0.25f, latitude / PI));

			temp.X = FMath::Cos(longitude + theta) * FMath::Sin(latitude);
			temp.Y = FMath::Cos(latitude);
			temp.Z = FMath::Sin(longitude + theta) * FMath::Sin(latitude);
			vertices.Add(temp); //normals.Add(temp);
			uv.Add(FVector2D(0.0f, 0.0f));
			//uv.Add(FVector2D(1.0f - (longitude + theta) / PI2 - 0.25f, latitude / PI));

			//FVector normal;
			//for (int i = 0; i < vertices.Num(); i++)
			//{
			//	normal += vertices[i];
			//}
			//normal /= vertices.Num();
			//for (int i = 0; i < vertices.Num(); i++)
			//{
			//	normals.Add(normal);
			//}

			Super::CreateMeshSection(count++, vertices, indices, normals, uv, TArray<FColor>(), TArray<FProcMeshTangent>(), false);
		}
	}
	UE_LOG(LogClass, Log, TEXT("Section Counter: %d"), Super::GetNumSections());
}