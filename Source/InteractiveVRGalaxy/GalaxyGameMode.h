// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "GalaxyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class INTERACTIVEVRGALAXY_API AGalaxyGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AGalaxyGameMode();

	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void SpawnOrbit(const FVector& location);

	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void DespawnOrbit() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit", meta = (AllowPrivateAccess = "true", DisplayName = "Orbit Class"))
	TSubclassOf<class ASolarSystem> m_OrbitClass;
};
