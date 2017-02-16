// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DomeMeshComponent.h"
#include "Dome.generated.h"

UENUM(BlueprintType)
enum class EDomeState : uint8
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

#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", DisplayName = "Mesh"))
	class UDomeMeshComponent *m_Mesh;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material", meta = (AllowPrivateAccess = "true", DisplayName = "Material"))
	UMaterialInterface *m_Material;

	UMaterialInstanceDynamic **m_MaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", meta = (AllowPrivateAccess = "true", DisplayName = "Next Dome State"))
	EDomeState m_NextDomeState;

	EDomeState m_DomeState;
	float m_DomeTimer;
	bool m_bTransparentClose;
};
