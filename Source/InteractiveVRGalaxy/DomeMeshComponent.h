// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMeshComponent.h"
#include "DomeMeshComponent.generated.h"

/**
 * UDomeMeshComponent represents the hemisphere mesh (- has no default material)
 */
UCLASS()
class INTERACTIVEVRGALAXY_API UDomeMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()
	
public:
	UDomeMeshComponent();

	void BeginPlay() override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dome", meta = (DisplayName = "Vertical Segments"))
	int m_VerticalSegments;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dome", meta = (DisplayName = "Horizontal Segments"))
	int m_HorizontalSegments;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dome", meta = (DisplayName = "Full Sphere"))
	bool m_bFullSphere;

private:
	UFUNCTION(BlueprintCallable, Category = "Dome")
	void CreateDome();
		
};
