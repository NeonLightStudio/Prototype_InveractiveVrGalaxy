// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CelestialBody.generated.h"

#define DAYS
#define HOURS
#define MINUTES
#define SECONDS
#define TIME_TO_SECONDS(days, hours, minutes, seconds) (((((days * 24.0f) + hours) * 60.0f) + minutes) * 60.0f + seconds)
#define SECONDS_TO_DAYS(seconds) (seconds * 60.0f * 60.0f * 24.0f)

UCLASS(abstract)
class INTERACTIVEVRGALAXY_API ACelestialBody : public AActor
{
	GENERATED_BODY()
	
protected:	
	// Sets default values for this actor's properties
	ACelestialBody();

public:
	virtual void BeginPlay() override;

#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// Set the scale of the body
	void SetScale(const float& size);

	float CalculateRotation(const float& radians) const;

	// Calculate the velocity of the body using the current angle (radians)
	float CalculateVelocity(const float& radians) const;

	// Calculate the distance from the center at the given angle (radians)
	float CalculateDistance(const float& radians) const;

	FVector CalculatePosition(const float& radians, const float& distanceOffset, const float& distanceScale) const;

	void Move(const float& multiplier, const float& distanceOffset, const float& distanceScale);

	FORCEINLINE USceneComponent* GetRootComponent() { return this->m_Root; }

	FORCEINLINE const float& GetAngleToCenter() const { return this->m_Angle; }

	FORCEINLINE const float& GetRadiusWithScale() const { return this->m_Root->Bounds.SphereRadius; }

private:
	// Minimum orbit speed (Speed at Aphelion)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit", meta = (AllowPrivateAccess = "true", DisplayName = "Min Speed (km/s)"))
	float m_MinSpeed;

	// Maximum orbit speed (Speed at Perihelion)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit", meta = (AllowPrivateAccess = "true", DisplayName = "Max Speed (km/s)"))
	float m_MaxSpeed;

	// Current orbit speed (Used only for debugging in the editor)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit", meta = (AllowPrivateAccess = "true", DisplayName = "Current Speed (km/s)"))
	float m_CurrentSpeed;

	// Maximum distance from the sun that this body reaches during its orbit (km)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit", meta = (AllowPrivateAccess = "true", DisplayName = "Aphelion (km)"))
	float m_Aphelion;

	// Minimum distance from the sun that this body reaches during its orbit (km)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit", meta = (AllowPrivateAccess = "true", DisplayName = "Perihelion (km)"))
	float m_Perihelion;

	// Semi-major axis of the ellipses orbit (km)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit", meta = (AllowPrivateAccess = "true", DisplayName = "Semi-major axis (km)"))
	float m_SemiMajorAxis;

	// Semi-minor axis of the ellipses orbit (km) (automatically calculated)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit", meta = (AllowPrivateAccess = "true", DisplayName = "Semi-minor axis (km)"))
	float m_SemiMinorAxis;

	// Eccentricity of the orbit 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Orbit", meta = (AllowPrivateAccess = "true", DisplayName = "Eccentricity"))
	float m_Eccentricity;

	// Perimeter of the orbit (km approximation) (automatically calculated)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit", meta = (AllowPrivateAccess = "true", DisplayName = "Orbit Perimeter (km)"))
	float m_Perimeter;

	// Angle of body to center
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit", meta = (AllowPrivateAccess = "true", DisplayName = "Orbit Angle (degrees)"))
	float m_Angle;

	// How many earth days it takes for the body to complete one full orbit around the center (earth days)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit", meta = (AllowPrivateAccess = "true", DisplayName = "Orbit Period (earth days)"))
	float m_OrbitPeriod;

	// Gravity on the surface of the body (m/s)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics", meta = (AllowPrivateAccess = "true", DisplayName = "Gravity (m/s)"))
	float m_Gravity;

	// Mass of the body (kg*10^24)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics", meta = (AllowPrivateAccess = "true", DisplayName = "Mass (kg x10^24)"))
	float m_Mass;

	// Radius of the body (km)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics", meta = (AllowPrivateAccess = "true", DisplayName = "Radius (km)"))
	float m_Radius;

	// Body tilt
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics", meta = (AllowPrivateAccess = "true", DisplayName = "Axial Tilt (degrees)"))
	float m_AxialTilt;

	// How long it takes for the body to rotate 360 degrees (earth seconds)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics", meta = (AllowPrivateAccess = "true", DisplayName = "Rotation Period (earth seconds)"))
	float m_RotationPeriod;

	// The current rotation of the planet (degrees) (Used only for debugging in the editor)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Characteristics", meta = (AllowPrivateAccess = "true", DisplayName = "Planet Rotation (degrees)"))
	float m_PlanetRotation;

public:
	FORCEINLINE const float& GetAphelion() const { return this->m_Aphelion; }

	FORCEINLINE const float& GetPerihelion() const { return this->m_Perihelion; }

	FORCEINLINE const float& GetSemiMajorAxis() const { return this->m_SemiMajorAxis; }

	FORCEINLINE const float& GetSemiMinorAxis() const { return this->m_SemiMinorAxis; }

	FORCEINLINE const float& GetEccentricity() const { return this->m_Eccentricity; }

	FORCEINLINE const float& GetPerimeter() const { return this->m_Perimeter; }

	FORCEINLINE const float& GetOrbitPeriod() const { return this->m_OrbitPeriod; }

	FORCEINLINE const float& GetGravity() const { return this->m_Gravity; }

	FORCEINLINE const float& GetMass() const { return this->m_Mass; }

	FORCEINLINE const float& GetRadius() const { return this->m_Radius; }

	FORCEINLINE const float& GetAxialTilt() const { return this->m_AxialTilt; }

	FORCEINLINE const float& GetRotationPeriod() const { return this->m_RotationPeriod; }

	FORCEINLINE void SetMinVelocity(const float& velocity) { this->m_MinSpeed = velocity; }

	FORCEINLINE void SetMaxVelocity(const float& velocity) { this->m_MaxSpeed = velocity; }

	FORCEINLINE void SetAphelion(const float& km) { this->m_Aphelion = km; }

	FORCEINLINE void SetPerihelion(const float& km) { this->m_Perihelion = km; }

	FORCEINLINE void SetSemiMajorAxis(const float& km) { this->m_SemiMajorAxis = km; this->CalculateSemiMinorAxis(); this->CalculatePerimeter(); }

	FORCEINLINE void SetEccentricity(const float& eccentricity) { this->m_Eccentricity = eccentricity; this->CalculateSemiMinorAxis(); this->CalculatePerimeter(); }

	FORCEINLINE void SetOrbitPeriod(const float& days) { this->m_OrbitPeriod = days; }

	FORCEINLINE void SetGravity(const float& ms) { this->m_Gravity = ms; }

	FORCEINLINE void SetMass(const float& kg24) { this->m_Mass = kg24; }

	FORCEINLINE void SetRadius(const float& km) { this->m_Radius = km; }

	FORCEINLINE void SetAxialTilt(const float& degrees) { this->m_AxialTilt = degrees; }

	FORCEINLINE void SetRotationPeriod(const float& seconds) { this->m_RotationPeriod = seconds; }

	UFUNCTION(BlueprintCallable, Category = "Time")
	static float CalculateTimeToSeconds(const float& days, const float& hours, const float& minutes, const float& seconds)
	{
		return TIME_TO_SECONDS(days, hours, minutes, seconds);
	}

private:
	FORCEINLINE void CalculateSemiMinorAxis() { this->m_SemiMinorAxis = this->m_SemiMajorAxis * FMath::Sqrt(1.0f - this->m_Eccentricity * this->m_Eccentricity); }

	FORCEINLINE void CalculatePerimeter() 
	{ 
		//Ramanujan approximation
		float h = FMath::Pow(this->m_SemiMajorAxis - this->m_SemiMinorAxis, 2.0f) / FMath::Pow(this->m_SemiMajorAxis + this->m_SemiMinorAxis, 2.0f);
		this->m_Perimeter = PI * (this->m_SemiMajorAxis + this->m_SemiMinorAxis) * (1.0f + (3.0f * h) / (10.0f + FMath::Sqrt(4 - 3.0f * h)));
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true", DisplayName = "Root"))
	UStaticMeshComponent *m_Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material", meta = (AllowPrivateAccess = "true", DisplayName = "Material"))
	UMaterialInterface *m_Material;
};
