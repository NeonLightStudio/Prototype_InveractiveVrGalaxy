// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DomeMeshComponent.h"
#include "Dome.generated.h"

UENUM(BlueprintType)
enum EDomeState
{
	Undefined,	// The dome has no state
	Open,		// The dome is completely transparent
	Transparent,// The dome panels go transparent without fully disappearing
	Close		// The dome is completely opaque
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
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetDomeState(const EDomeState& state);

	// Get the current state of the dome. The state returned is (potentially) from the previous tick and not the current.
	UFUNCTION(BlueprintCallable, Category = "State")
	const EDomeState& GetDomeState() const { return this->m_DomeState; }

private:
	UPROPERTY()
	UMaterial *m_Material;
	UMaterialInstanceDynamic **m_MaterialInstance;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class UDomeMeshComponent *m_Mesh;

	float m_DomeTimer;
	bool m_bTransparentClose;

	//float m_DomeStateTime;
	//bool m_DomeStateComplete;

	EDomeState m_DomeState, m_NextDomeState;
};
