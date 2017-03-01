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

	UFUNCTION()
	void DespawnSystem();

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// Spawns an array of bodies and sets their parent to parent. This method is used recursively.
	void SpawnBodies(AActor *parent, const TArray<TSubclassOf<ACelestialBody>>& bodies);
	
	// Called every frame
	virtual void Tick(float delta) override;

	UFUNCTION(BlueprintCallable, Category = "Scale")
	void SetTimeScale(const float& scale) { this->m_TimeScale = scale; }

	UFUNCTION(BlueprintCallable, Category = "Scale")
	void SetRadiusScale(const float& scale) { this->m_RadiusScale = scale; this->bScaleUpdateRequired = true; }

	UFUNCTION(BlueprintCallable, Category = "Scale")
	void SetOrbitDistanceScale(const float& scale) { this->m_OrbitDistanceScale = scale; this->bScaleUpdateRequired = true; }

private:
	// The root component of the solar system. It's just an empty SceneComponent.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root", meta = (AllowPrivateAccess = "true", DisplayName = "Root"))
	USceneComponent *m_Root;

	// The center body of the solar system. Typically a sun, but can be anything.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planets", meta = (AllowPrivateAccess = "true", DisplayName = "System Center (sun)"))
	TSubclassOf<ACelestialBody> m_Center;

	// An array of Celestial Bodies which make up this solar system. You should put blueprints in here. (Solar System will spawn them when the game begins)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planets", meta = (AllowPrivateAccess = "true", DisplayName = "Celestial Bodies"))
	TArray<TSubclassOf<ACelestialBody>> m_CelestialBodies;

	// How fast should time move. (1.0 = real time)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scale", meta = (AllowPrivateAccess = "true", DisplayName = "Time Scale"))
	float m_TimeScale;

	// This scale multiplier will be applied to the radius of every body attached to this system. (1.0 = actual scale)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scale", meta = (AllowPrivateAccess = "true", DisplayName = "Size Scale"))
	float m_RadiusScale;

	// This scale multiplier will be applied to every body attached to this system. It can be used to bring bodies closer to the center. (1.0 = actual scale)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scale", meta = (AllowPrivateAccess = "true", DisplayName = "Distance Scale"))
	float m_OrbitDistanceScale;

	// The radius of the center actor. It is required to work out distances of attached bodies
	float m_CenterOffset;

	// The center actor
	ACelestialBody *m_CenterActor;

	// An array containing all of the attached bodies. (Cached for efficency)
	TArray<AActor*> m_AttachedBodies;

	// Whether or not the scale has been modified
	bool bScaleUpdateRequired;
};
