// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "DebugCharacter.generated.h"

UCLASS()
class INTERACTIVEVRGALAXY_API ADebugCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADebugCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float delta) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void OnAction();

	void MoveForward(float value);

	void MoveSideways(float value);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true", DisplayName = "Camera Component"))
	class UCameraComponent* m_FirstPersonCameraComponent;

	// Dome reference. This field is set within the blueprint.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dome", meta = (AllowPrivateAccess = "true", DisplayName = "Dome Actor"))
	class ADome *m_Dome;
	
public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (DisplayName = "Base Turn Rate"))
	float m_BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (DisplayName = "Base Look Up Rate"))
	float m_BaseLookUpRate;

};
