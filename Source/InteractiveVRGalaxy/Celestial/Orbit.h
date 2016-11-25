// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Orbit.generated.h"

UCLASS()
class INTERACTIVEVRGALAXY_API AOrbit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOrbit();

	virtual void Tick(float delta) override;

	void SetColor(FLinearColor color);

	void SetRadius(const float& radius);

	void SetPoints(TArray<FVector> points);

	void UpdateOrbit();

private:
	UPROPERTY()
	UMaterial *m_Material;

	UPROPERTY()
	FLinearColor m_Color;

	UPROPERTY()
	class UOrbitMeshComponent *m_Mesh;

	bool m_bMeshUpdateRequired, m_bMaterialUpdateRequired;
};
