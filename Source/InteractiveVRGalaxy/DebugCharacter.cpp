// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveVRGalaxy.h"

#include "Dome.h"
#include "DebugCharacter.h"


// Sets default values
ADebugCharacter::ADebugCharacter() : m_Dome(nullptr)
{
	Super::PrimaryActorTick.bCanEverTick = true;

	Super::GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	this->m_BaseTurnRate = 45.f;
	this->m_BaseLookUpRate = 45.f;

	// create a CameraComponent	
	this->m_FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	this->m_FirstPersonCameraComponent->SetupAttachment(Super::GetCapsuleComponent());
	this->m_FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	this->m_FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Super::GetCharacterMovement()->MaxWalkSpeed = 800.0f;
}

// Called when the game starts or when spawned
void ADebugCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADebugCharacter::Tick(float delta)
{
	Super::Tick(delta);
}

// Called to bind functionality to input
void ADebugCharacter::SetupPlayerInputComponent(class UInputComponent* input)
{
	check(input);

	//input->BindAction(FName("Interact"), EInputEvent::IE_Pressed, this, )
	input->BindAxis("LookX", this, &APawn::AddControllerYawInput);
	input->BindAxis("LookY", this, &APawn::AddControllerPitchInput);

	input->BindAxis("MoveForward", this, &ADebugCharacter::MoveForward);
	input->BindAxis("MoveSideways", this, &ADebugCharacter::MoveSideways);

	input->BindAction("Action", EInputEvent::IE_Pressed, this, &ADebugCharacter::OnAction);
}


void ADebugCharacter::OnAction()
{
	check(this->m_Dome);
	this->m_Dome->NextDomeState();
}

void ADebugCharacter::MoveForward(float value)
{
	if (value != 0.0f)
	{
		Super::AddMovementInput(Super::GetActorForwardVector(), value);
	}
}

void ADebugCharacter::MoveSideways(float value)
{
	if (value != 0.0f)
	{
		Super::AddMovementInput(Super::GetActorRightVector(), value);
	}
}