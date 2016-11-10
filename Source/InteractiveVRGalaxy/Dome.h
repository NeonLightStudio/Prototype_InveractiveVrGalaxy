// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DomeMeshComponent.h"
#include "Dome.generated.h"

UENUM()
enum class EDomeState : uint8
{
	Opaque = 0,
	Transparent = 1
};

UCLASS()
class INTERACTIVEVRGALAXY_API ADome : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADome();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float delta) override;

	//So can be called in blueprints 
	UFUNCTION(BlueprintCallable, Category = "Dome")
	void NextDomeState();

	inline const EDomeState& GetDomeState() const { return this->m_DomeState; }

private:
	UPROPERTY(EditAnywhere)
	class UDomeMeshComponent *m_Mesh;

	EDomeState m_DomeState;
};
