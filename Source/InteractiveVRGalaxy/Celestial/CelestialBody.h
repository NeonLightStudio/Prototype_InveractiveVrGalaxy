// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Atmosphere.h"
#include "Orbit.h"
#include "CelestialBody.generated.h"

#define DAYS
#define HOURS
#define MINUTES
#define SECONDS
#define TIME_TO_SECONDS(days, hours, minutes, seconds) (((((days * 24.0f) + hours) * 60.0f) + minutes) * 60.0f + seconds)
#define SECONDS_TO_DAYS(seconds) (seconds * 60.0f * 60.0f * 24.0f)

#define MATERIAL_NAME_SUN_LOCATION FName("Sun Location")

UCLASS(abstract)
class INTERACTIVEVRGALAXY_API ACelestialBody : public AActor
{
	GENERATED_BODY()
	
protected:	
	// Sets default values for this actor's properties
	ACelestialBody();

public:
	virtual void BeginDestroy() override;

	virtual void BeginPlay() override;

#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// Set the scale of the body
	void SetScale(const float& size);

	// Calculate the rotation of the body at the given orbit angle (radians)
	float CalculateRotation(const float& radians) const;

	// Calculate the velocity of the body using the current angle (radians)
	float CalculateVelocity(const float& radians) const;

	// Calculate the distance from the center at the given angle (radians)
	float CalculateDistance(const float& radians) const;

	// Calculate the position of the body using the orbit angle, distance offset, and distance scale
	FVector CalculatePosition(const float& radians, const float& offset, const float& distanceScale) const;

	// If this body has an orbit actor, disable and enable it again. (should be called if something makes the orbit no longer accurate)
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void ResetDrawOrbit();

	// Whether or not to draw the orbit of this body
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void SetDrawOrbit(const bool& draw);

	// Whether or not the draw the atmosphere of this body
	UFUNCTION(BlueprintCallable, Category = "Atmosphere")
	void SetDrawAtmosphere(const bool& enable);

	// Move this body along its orbit. Requires the center, speed multiplier, and orbit distance scale
	void Move(const ACelestialBody *center, const float& timeScale, const float& distanceScale, const float& delta);

	FORCEINLINE UStaticMeshComponent* GetRootComponent() const { return this->m_Root; }

	FORCEINLINE const TArray<TSubclassOf<ACelestialBody>>& GetSatellites() const { return this->m_Satellites; }

	FORCEINLINE const float& GetAngleToCenter() const { return this->m_Angle; }

	FORCEINLINE const float& GetRadiusWithScale() const { return this->m_Root->Bounds.SphereRadius; }

	FORCEINLINE const float& GetSolarSystemRadiusScale() const { return this->m_LastRadiusScale; }

	FORCEINLINE const float& GetSolarSystemDistanceScale() const { return this->m_LastDistanceScale; }

private:
	FORCEINLINE void CalculateSemiMinorAxis() { this->m_SemiMinorAxis = this->m_SemiMajorAxis * FMath::Sqrt(1.0f - this->m_Eccentricity * this->m_Eccentricity); }

	FORCEINLINE void CalculatePerimeter()
	{
		//Ramanujan approximation
		float h = FMath::Pow(this->m_SemiMajorAxis - this->m_SemiMinorAxis, 2.0f) / FMath::Pow(this->m_SemiMajorAxis + this->m_SemiMinorAxis, 2.0f);
		this->m_Perimeter = PI * (this->m_SemiMajorAxis + this->m_SemiMinorAxis) * (1.0f + (3.0f * h) / (10.0f + FMath::Sqrt(4 - 3.0f * h)));
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true", DisplayName = "Mesh"))
	UStaticMesh *m_Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true", DisplayName = "Root"))
	UStaticMeshComponent *m_Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material", meta = (AllowPrivateAccess = "true", DisplayName = "Material"))
	UMaterialInterface *m_Material;

	UPROPERTY()
	UMaterialInstanceDynamic *m_MaterialDynamic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Satellites", meta = (AllowPrivateAccess = "true", DisplayName = "Satellites"))
	TArray<TSubclassOf<ACelestialBody>> m_Satellites;

	// Parameters stored within the solar system are cached here. May not be latest value and can be changed without notice.
	float m_LastOffset, m_LastRadiusScale, m_LastDistanceScale;

private:
	// Whether or not to have an atmosphere
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atmosphere", meta = (AllowPrivateAccess = "true", DisplayName = "Enable Atmosphere"))
	bool m_bDrawAtmosphere;

	// Atmosphere data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atmosphere", meta = (AllowPrivateAccess = "true", DisplayName = "Data"))
	FAtmosphereData m_AtmosphereData;

	// Atmosphere actor
	UPROPERTY()
	AAtmosphere *m_Atmosphere;

	UPROPERTY()
	AOrbit *m_Orbit;

	// Whether or not to draw the orbit
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitRender", meta = (AllowPrivateAccess = "true", DisplayName = "Draw Orbit"))
	bool m_bDrawOrbit;

	// Whether or not to draw the orbit
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitRender", meta = (AllowPrivateAccess = "true", DisplayName = "Draw Radius"))
	float m_DrawOrbitRadius;

	// How many samples to take for the orbit render
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OrbitRender", meta = (AllowPrivateAccess = "true", DisplayName = "Draw Resolution"))
	int m_DrawOrbitResolution;

	// Color of the orbit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OrbitRender", meta = (AllowPrivateAccess = "true", DisplayName = "Draw Color"))
	FLinearColor m_DrawOrbitColor;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit", meta = (AllowPrivateAccess = "true", DisplayName = "Aphelion/Apogee (km)"))
	float m_FurthestDistance;

	// Minimum distance from the sun that this body reaches during its orbit (km)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit", meta = (AllowPrivateAccess = "true", DisplayName = "Perihelion/Perigee (km)"))
	float m_NearestDistance;

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

	// How many earth days it takes for the body to complete one full orbit around the center (earth days)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit", meta = (AllowPrivateAccess = "true", DisplayName = "Rotate Clockwise (orbit)"))
	bool m_RotateOrbitClockwise;

	// Velocity scale multiplier (1.0 = default. Touching this will completely mess up the accuracy of days per year)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scale", meta = (AllowPrivateAccess = "true", DisplayName = "Velocity Scale (%)"))
	float m_VelocityScale;

	// Rotation scale multiplier (1.0 = default. Touching this will completely mess up the accuracy of days per year)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scale", meta = (AllowPrivateAccess = "true", DisplayName = "Rotation Scale (%)"))
	float m_RotationScale;

	// Planet radius multiplier (1.0f = default according to solar system object)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scale", meta = (AllowPrivateAccess = "true", DisplayName = "Radius Scale (%)"))
	float m_RadiusScale;

	// Orbit distance multiplier (1.0 = default distance according to solar system object)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scale", meta = (AllowPrivateAccess = "true", DisplayName = "Orbit Distance Scale (%)"))
	float m_OrbitDistanceScale;

	// Gravity on the surface of the body (m/s)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics", meta = (AllowPrivateAccess = "true", DisplayName = "Gravity (m/s)"))
	float m_Gravity;

	// Mass of the body (kg*10^24)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics", meta = (AllowPrivateAccess = "true", DisplayName = "Mass (kg x10^24)"))
	float m_Mass;

	// Radius of the body (km)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Characteristics", meta = (AllowPrivateAccess = "true", DisplayName = "Radius (km)"))
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

	// Whether or not to rotate the planet clockwise
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics", meta = (AllowPrivateAccess = "true", DisplayName = "Rotate Clockwise (planet)"))
	bool m_RotatePlanetClockwise;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World", meta = (AllowPrivateAccess = "true", DisplayName = "Sun Location (world space)"))
	FVector m_SunLocation;

	// Whether or not to move and rotate the body
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true", DisplayName = "Move Body"))
	bool m_bMoveBody;
	bool m_bMoveBodyTransition; // should be true if the body is in transition mode

	// Time taken for the body to return to its proper position after MoveBody is toggled (seconds)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true", DisplayName = "Transition Delay (seconds)"))
	float m_TransitionDelay;
	float m_TransitionTimer;
	FVector m_TransitionPosStart;
	FRotator m_TransitionRotStart;

public:
	UFUNCTION(BlueprintPure, Category = "Atmosphere")
	AAtmosphere* GetAtmosphere() const { return this->m_Atmosphere; }

	UFUNCTION(BlueprintPure, Category = "Atmosphere")
	FAtmosphereData& GetAtmosphereData() { return this->m_AtmosphereData; }

	UFUNCTION(BlueprintPure, Category = "Orbit")
	const float& GetFurthestDistance() const { return this->m_FurthestDistance; }

	UFUNCTION(BlueprintPure, Category = "Orbit")
	const float& GetNearestDistance() const { return this->m_NearestDistance; }

	UFUNCTION(BlueprintPure, Category = "Orbit")
	const float& GetSemiMajorAxis() const { return this->m_SemiMajorAxis; }

	UFUNCTION(BlueprintPure, Category = "Orbit")
	const float& GetSemiMinorAxis() const { return this->m_SemiMinorAxis; }

	UFUNCTION(BlueprintPure, Category = "Orbit")
	const float& GetEccentricity() const { return this->m_Eccentricity; }

	UFUNCTION(BlueprintPure, Category = "Orbit")
	const float& GetPerimeter() const { return this->m_Perimeter; }

	UFUNCTION(BlueprintPure, Category = "Orbit")
	const float& GetOrbitPeriod() const { return this->m_OrbitPeriod; }

	UFUNCTION(BlueprintPure, Category = "Characteristics")
	const float& GetGravity() const { return this->m_Gravity; }

	UFUNCTION(BlueprintPure, Category = "Characteristics")
	const float& GetMass() const { return this->m_Mass; }

	UFUNCTION(BlueprintPure, Category = "Characteristics")
	const float& GetRadius() const { return this->m_Radius; }

	UFUNCTION(BlueprintPure, Category = "Characteristics")
	const float& GetAxialTilt() const { return this->m_AxialTilt; }

	UFUNCTION(BlueprintPure, Category = "Characteristics")
	const float& GetRotationPeriod() const { return this->m_RotationPeriod; }

	UFUNCTION(BlueprintPure, Category = "Scale")
	const float& GetVelocityScale() const { return this->m_VelocityScale; }

	UFUNCTION(BlueprintPure, Category = "Scale")
	const float& GetRotationScale() const { return this->m_RotationScale; }

	UFUNCTION(BlueprintPure, Category = "Scale")
	const float& GetRadiusScale() const { return this->m_RadiusScale; }

	UFUNCTION(BlueprintPure, Category = "Scale")
	const float& GetOrbitDistanceScale() const { return this->m_OrbitDistanceScale; }

	UFUNCTION(BlueprintPure, Category = "World")
	const FVector& GetSunLocation() const { return this->m_SunLocation; }

	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void SetMinVelocity(const float& velocity) { this->m_MinSpeed = velocity; }

	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void SetMaxVelocity(const float& velocity) { this->m_MaxSpeed = velocity; }

	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void SetFurthestDistance(const float& km) { this->m_FurthestDistance = km; }

	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void SetNearestDistance(const float& km) { this->m_NearestDistance = km; }

	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void SetSemiMajorAxis(const float& km) { this->m_SemiMajorAxis = km; this->CalculateSemiMinorAxis(); this->CalculatePerimeter(); }

	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void SetEccentricity(const float& eccentricity) { this->m_Eccentricity = eccentricity; this->CalculateSemiMinorAxis(); this->CalculatePerimeter(); }

	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void SetOrbitPeriod(const float& days) { this->m_OrbitPeriod = days; }

	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void SetRotateOrbitClockwise(const bool& clockwise) { this->m_RotateOrbitClockwise = clockwise; }

	UFUNCTION(BlueprintCallable, Category = "Characteristics")
	void SetGravity(const float& ms) { this->m_Gravity = ms; }

	UFUNCTION(BlueprintCallable, Category = "Characteristics")
	void SetMass(const float& kg24) { this->m_Mass = kg24; }

	UFUNCTION(BlueprintCallable, Category = "Characteristics")
	void SetRadius(const float& km) { this->m_Radius = km; this->SetScale(this->m_LastRadiusScale); }

	UFUNCTION(BlueprintCallable, Category = "Characteristics")
	void SetAxialTilt(const float& degrees) { this->m_AxialTilt = degrees; }

	UFUNCTION(BlueprintCallable, Category = "Characteristics")
	void SetRotationPeriod(const float& seconds) { this->m_RotationPeriod = seconds; }

	UFUNCTION(BlueprintCallable, Category = "Characteristics")
	void SetRotatePlanetClockwise(const bool& clockwise) { this->m_RotatePlanetClockwise = clockwise; }

	UFUNCTION(BlueprintCallable, Category = "Scale")
	void SetVelocityScale(const float& scale) { this->m_VelocityScale = scale; }

	UFUNCTION(BlueprintCallable, Category = "Scale")
	void SetRotationScale(const float& scale) { this->m_RotationScale = scale; }

	UFUNCTION(BlueprintCallable, Category = "Scale")
	void SetRadiusScale(const float& scale) { this->m_RadiusScale = scale; this->SetScale(this->m_LastRadiusScale); }

	UFUNCTION(BlueprintCallable, Category = "Scale")
	void SetOrbitDistanceScale(const float& scale) { this->m_OrbitDistanceScale = scale; }

	UFUNCTION(BlueprintCallable, Category = "Scale")
	void SetSunLocation(const FVector& location);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetMoveBody(const bool& move)
	{
		this->m_bMoveBody = move; 
		this->m_bMoveBodyTransition = move; 
		if (move)
		{
			this->m_TransitionTimer = 0.0f;
			this->m_TransitionPosStart = this->m_Root->GetComponentLocation();
			this->m_TransitionRotStart = this->m_Root->GetComponentRotation();
		}
	}

	UFUNCTION(BlueprintCallable, Category = "Time")
	static float CalculateTimeToSeconds(const float& days, const float& hours, const float& minutes, const float& seconds)
	{
		return TIME_TO_SECONDS(days, hours, minutes, seconds);
	}
};
