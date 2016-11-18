// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Atmosphere.generated.h"

USTRUCT()
struct FAtmosphereData 
{
	GENERATED_BODY()

	FAtmosphereData() : m_Height(12.0f), m_HeightScale(1.0f), m_Color(FLinearColor::White) {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true", DisplayName = "Height"))
	float m_Height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true", DisplayName = "Height Scale"))
	float m_HeightScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true", DisplayName = "Color"))
	FLinearColor m_Color;
};

UCLASS()
class INTERACTIVEVRGALAXY_API AAtmosphere : public AActor
{
	GENERATED_BODY()
	
public:	
	AAtmosphere();

	void BeginPlay() override;

	void UpdateAtmosphere() const;

	FORCEINLINE void SetAtmosphereData(FAtmosphereData *data) { this->m_Data = data; }
	
private:
	UStaticMeshComponent *m_Root;
	UMaterialInstanceDynamic *m_Material;

	FAtmosphereData *m_Data;
};
