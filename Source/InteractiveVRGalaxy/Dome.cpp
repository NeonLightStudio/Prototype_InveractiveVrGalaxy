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

#define DOME_MATERIAL_LOCATION TEXT("Material'/Game/VirtualRealityBP/Materials/M_DomeMaterial.M_DomeMaterial'")

// Sets default values
ADome::ADome() : m_MaterialInstance(nullptr), m_NextDomeState(EDomeState::Undefined), m_DomeState(EDomeState::Close)
{
	// Let the dome tick so that we can control the animation
	Super::PrimaryActorTick.bCanEverTick = true;

	// Set the mesh of this component to the Dome Mesh
	this->m_Mesh = UObject::CreateDefaultSubobject<UDomeMeshComponent>(TEXT("DomeMesh"));
	Super::RootComponent = this->m_Mesh;

	// Find our Dome Material
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(DOME_MATERIAL_LOCATION);
	if (Material.Succeeded())
	{
		this->m_Material = Material.Object;
	}
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

		this->m_Mesh->SetMaterial(i, material);
		this->m_MaterialInstance[i] = material;
	}
}

void ADome::SetDomeState(const EDomeState& state)
{
	this->m_DomeTimer = 0.0f;
	this->m_NextDomeState = state;
	this->m_bTransparentClose = state == EDomeState::Close
		&& this->m_DomeState == EDomeState::Transparent;
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
			if (this->m_NextDomeState == EDomeState::Close)
			{
				color = 0.0f;
				time = ANIMATION_DURATION - time;
			}
			displace = FMath::Clamp(((time - startDelay) - (ANIMATION_DURATION - displaceDuration)) / (displaceDuration - startDelay), 0.0f, 1.0f);
			fade = FMath::Clamp((time - startDelay) / colorDuration, 0.0f, 1.0f);
			if (this->m_NextDomeState == EDomeState::Transparent || this->m_bTransparentClose)
			{
				color = 0.0f;
				displace = 0.0f;
			}

			material->SetScalarParameterValue(SCALAR_COLOR_MULTIPLIER, color);
			material->SetScalarParameterValue(SCALAR_DISPLACE_PERCENTAGE, displace);
			material->SetScalarParameterValue(SCALAR_FADE_PERCENTAGE, fade);
		}
	}
}

#if WITH_EDITOR
void ADome::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName name = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (name == GET_MEMBER_NAME_CHECKED(ADome, m_NextDomeState))
	{
		this->SetDomeState(this->m_NextDomeState);
	}
}
#endif