// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DomeControl.generated.h"

UCLASS()
class INTERACTIVEVRGALAXY_API ADomeControl : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADomeControl();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "Dome")
	void OpenDome();
	UFUNCTION( BlueprintCallable, Category = "Dome" )
	void CloseDome();
	UFUNCTION(BlueprintCallable, Category = "Dome")
	void Transparent();
	UFUNCTION(BlueprintCallable, Category = "Dome")
	void Opaque();

private:

	// Dome reference. This field is set within the blueprint.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dome", meta = (AllowPrivateAccess = "true", DisplayName = "Dome Actor"))
	class ADome *m_Dome;
	
	
};
