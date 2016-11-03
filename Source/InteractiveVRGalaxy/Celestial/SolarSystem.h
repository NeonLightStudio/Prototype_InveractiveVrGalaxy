// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CelestialBody.h"

#include "GameFramework/Actor.h"
#include "SolarSystem.generated.h"

UCLASS()
class INTERACTIVEVRGALAXY_API ASolarSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASolarSystem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	void SetScale(const float& size, const float& distance);
	
	// Called every frame
	virtual void Tick(float delta) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root", meta = (AllowPrivateAccess = "true", DisplayName = "Root"))
	USceneComponent *m_Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planets", meta = (AllowPrivateAccess = "true", DisplayName = "System Center (sun)"))
	TSubclassOf<ACelestialBody> m_Center;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planets", meta = (AllowPrivateAccess = "true", DisplayName = "Celestial Bodies"))
	TArray<TSubclassOf<ACelestialBody>> m_CelestialBodies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", meta = (AllowPrivateAccess = "true", DisplayName = "Velocity Multiplier"))
	float m_VelocityMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", meta = (AllowPrivateAccess = "true", DisplayName = "Size Scale"))
	float m_SizeScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", meta = (AllowPrivateAccess = "true", DisplayName = "Distance Scale"))
	float m_DistanceScale;

	float m_CenterOffset;
	ACelestialBody *m_CenterActor;
	TArray<AActor*> m_AttachedActors;

	bool bScaleUpdateRequired;
};
