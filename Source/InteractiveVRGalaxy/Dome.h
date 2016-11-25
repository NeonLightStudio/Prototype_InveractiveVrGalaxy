// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DomeMeshComponent.h"
#include "Dome.generated.h"

//UENUM()
enum EDomeState : uint8
{
	Undefined = 0,	// The dome has no state
	Open = 1,		// The dome is completely transparent
	Close = 2		// The dome is completely opaque
};

UCLASS()
class INTERACTIVEVRGALAXY_API ADome : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADome();
	~ADome();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float delta) override;

	// Set the state of the dome. It will be set within the next game tick.
	FORCEINLINE void SetDomeState(const EDomeState& state) { this->m_NextDomeState = state; }

	// Get the current state of the dome. The state returned is (potentially) from the previous tick and not the current.
	FORCEINLINE const EDomeState& GetDomeState() const { return this->m_DomeState; }

private:
	UPROPERTY()
	UMaterial *m_Material;
	UMaterialInstanceDynamic **m_MaterialInstance;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class UDomeMeshComponent *m_Mesh;

	float m_DomeStateTime;
	//bool m_DomeStateComplete;

	EDomeState m_DomeState, m_NextDomeState;
};
