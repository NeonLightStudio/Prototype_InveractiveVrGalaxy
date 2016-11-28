// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveVRGalaxy.h"

#include "Dome.h"

#define ANIMATION_OPEN 1.0
#define ANIMATION_CLOSE 0.0

#define ANIMATION_DURATION 2.6f //this number must be constant
#define ANIMATION_START_DELAY FMath::RandRange(0.2f, 0.8f)
#define ANIMATION_COLOR_BLEND_DURATION FMath::RandRange(0.4f, 1.8f)
#define ANIMATION_DISPLACE_DURATION 1.0f
#define ANIMATION_DISPLACE_OFFSET 175.0f
#define ANIMATION_LOAD_COLOR FLinearColor(0.1f, 0.2f, 0.6f, 1.0f)

#define SCALAR_FADE_PERCENTAGE FName("Fade Percentage")
#define SCALAR_DISPLACE_PERCENTAGE FName("Displace Percentage")
#define SCALAR_DISPLACE_OFFSET FName("Displace Offset")
#define VECTOR_LOAD_COLOR FName("Load Color")
#define SCALAR_COLOR_MULTIPLIER FName("Color Multiplier")
// Constant values
#define SCALAR_ANIMATION_START_DELAY FName("Animation Start Delay")
#define SCALAR_ANIMATION_COLOR_BLEND_DURATION FName("Color Blend Duration")
#define SCALAR_ANIMATION_DISPLACE_DURATION FName("Displace Duration")

//#define SCALAR_OPEN FName("Open")
//#define SCALAR_TIME FName("Time")
//#define SCALAR_ANIMATION_DURATION				FName("Animation Duration")
//#define SCALAR_ANIMATION_START_DELAY			FName("Animation Start Delay")
//#define SCALAR_ANIMATION_COLOR_BLEND_DURATION	FName("Color Blend Duration")
//#define SCALAR_ANIMATION_DISPLACE_DURATION		FName("Displace Duration")
//#define SCALAR_ANIMATION_DISPLACE_OFFSET		FName("Displace Offset")

#define DOME_MATERIAL_LOCATION "Material'/Game/VirtualRealityBP/Materials/M_DomeMaterial.M_DomeMaterial'"

// Sets default values
ADome::ADome() : m_MaterialInstance(nullptr), /*m_DomeStateTime(0.0f),*/ m_DomeState(EDomeState::Close), m_NextDomeState(EDomeState::Undefined)
{
	// Set the mesh of this component to the Dome Mesh
	this->m_Mesh = UObject::CreateDefaultSubobject<UDomeMeshComponent>(TEXT("DomeMesh"));
	Super::RootComponent = this->m_Mesh;

	// Find our Dome Material
	static ConstructorHelpers::FObjectFinder<UMaterial> material(TEXT(DOME_MATERIAL_LOCATION));
	check(material.Succeeded());
	this->m_Material = material.Object;

	// Let the dome tick so that we can control the animation
	Super::PrimaryActorTick.bCanEverTick = true;
}

ADome::~ADome()
{
	if (this->m_MaterialInstance != nullptr)
	{
		delete[] this->m_MaterialInstance;
		this->m_MaterialInstance = nullptr;
	}
}

// Called when the game starts or when spawned
void ADome::BeginPlay()
{
	Super::BeginPlay();

	check(this->m_Mesh);
	if (this->m_Mesh->GetNumSections() == 0)
	{
		return;
	}
	check(this->m_Material);

	if (this->m_MaterialInstance != nullptr)
	{
		delete[] this->m_MaterialInstance;
	}
	this->m_MaterialInstance = new UMaterialInstanceDynamic*[this->m_Mesh->GetNumSections()];

	// Since this is the inital setup we will subtract the animation duration so that the animation does not play on initialization
	//this->m_DomeStateTime = UGameplayStatics::GetRealTimeSeconds(this) - ANIMATION_DURATION;
	for (int i = 0; i < this->m_Mesh->GetNumSections(); i++)
	{
		UMaterialInstanceDynamic *material = UMaterialInstanceDynamic::Create(this->m_Material, this);

		material->SetScalarParameterValue(SCALAR_FADE_PERCENTAGE, 0.0f);
		material->SetScalarParameterValue(SCALAR_DISPLACE_PERCENTAGE, 0.0f);
		material->SetScalarParameterValue(SCALAR_DISPLACE_OFFSET, ANIMATION_DISPLACE_OFFSET);
		material->SetVectorParameterValue(VECTOR_LOAD_COLOR, ANIMATION_LOAD_COLOR);
		material->SetScalarParameterValue(SCALAR_COLOR_MULTIPLIER, 1.0f);

		// Constants
		material->SetScalarParameterValue(SCALAR_ANIMATION_START_DELAY, ANIMATION_START_DELAY);
		material->SetScalarParameterValue(SCALAR_ANIMATION_COLOR_BLEND_DURATION, ANIMATION_COLOR_BLEND_DURATION);
		material->SetScalarParameterValue(SCALAR_ANIMATION_DISPLACE_DURATION, ANIMATION_DISPLACE_DURATION);

		//material->SetScalarParameterValue(SCALAR_OPEN, ANIMATION_CLOSE);
		//material->SetScalarParameterValue(SCALAR_ANIMATION_DURATION, ANIMATION_DURATION);
		//material->SetScalarParameterValue(SCALAR_ANIMATION_START_DELAY, ANIMATION_START_DELAY);
		//material->SetScalarParameterValue(SCALAR_ANIMATION_COLOR_BLEND_DURATION, ANIMATION_COLOR_BLEND_DURATION);
		//material->SetScalarParameterValue(SCALAR_ANIMATION_DISPLACE_DURATION, ANIMATION_DISPLACE_DURATION);
		//material->SetScalarParameterValue(SCALAR_ANIMATION_DISPLACE_OFFSET, ANIMATION_DISPLACE_OFFSET);

		//// Set the time which our animation started to a time where it should already be finished
		//material->SetScalarParameterValue(SCALAR_TIME, this->m_DomeStateTime); 

		this->m_Mesh->SetMaterial(i, material);
		this->m_MaterialInstance[i] = material;
	}
}

void ADome::SetDomeState(const EDomeState& state)
{
	this->m_DomeTimer = 0.0f;
	this->m_NextDomeState = state;
}

// Called every frame
void ADome::Tick(float delta)
{
	Super::Tick(delta);

	if (this->m_NextDomeState != this->m_DomeState && this->m_NextDomeState != EDomeState::Undefined)
	{
		this->m_DomeTimer += delta;
		if(this->m_DomeTimer >= ANIMATION_DURATION)
		{
			this->m_DomeTimer = ANIMATION_DURATION;
			this->m_DomeState = this->m_NextDomeState;
		}

		for (int i = 0; i < this->m_Mesh->GetNumSections(); i++)
		{
			UMaterialInstanceDynamic *material = this->m_MaterialInstance[i];
			float startDelay, colorDuration, displaceDuration;
			material->GetScalarParameterValue(SCALAR_ANIMATION_START_DELAY, startDelay);
			material->GetScalarParameterValue(SCALAR_ANIMATION_COLOR_BLEND_DURATION, colorDuration);
			material->GetScalarParameterValue(SCALAR_ANIMATION_DISPLACE_DURATION, displaceDuration);

			float color = 1.0f, displace = 0.0f, fade = 0.0f;
			float time = this->m_DomeTimer;
			if(this->m_NextDomeState == EDomeState::Close)
			{
				color = 0.0f;
				time = ANIMATION_DURATION - time;
			}
			displace = FMath::Clamp(((time - startDelay) - (ANIMATION_DURATION - displaceDuration)) / displaceDuration, 0.0f, 1.0f);
			fade = FMath::Clamp((time - startDelay) / colorDuration, 0.0f, 1.0f);
			if (this->m_NextDomeState == EDomeState::Transparent)
			{
				color = 0.0f;
				displace = 0.0f;
			}

			material->SetScalarParameterValue(SCALAR_COLOR_MULTIPLIER, color);
			material->SetScalarParameterValue(SCALAR_DISPLACE_PERCENTAGE, displace);
			material->SetScalarParameterValue(SCALAR_FADE_PERCENTAGE, fade);
		}
	}


	//if (this->m_NextDomeState != this->m_DomeState
	//	&& this->m_NextDomeState != EDomeState::Undefined)
	//{
		//float time = UGameplayStatics::GetRealTimeSeconds(this);
		//float timeOffset = (this->m_DomeStateTime + ANIMATION_DURATION) - time;

		//if (timeOffset > 0.0f && timeOffset < ANIMATION_DURATION)
		//{
		//	time -= timeOffset;
		//}
		//check(this->m_MaterialInstance);
		//for (int i = 0; i < this->m_Mesh->GetNumSections(); i++)
		//{
		//	UMaterialInstanceDynamic *material = this->m_MaterialInstance[i];
		//	check(material);
		//	material->SetScalarParameterValue(SCALAR_OPEN, this->m_NextDomeState == EDomeState::Open ? ANIMATION_OPEN : ANIMATION_CLOSE);
		//	material->SetScalarParameterValue(SCALAR_TIME, time);
		//}
		//this->m_DomeStateTime = time;
		//this->m_DomeState = this->m_NextDomeState;
		//this->m_NextDomeState = EDomeState::Undefined;
	//}
}
