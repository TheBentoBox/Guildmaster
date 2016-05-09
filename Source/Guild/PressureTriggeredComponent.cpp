// Fill out your copyright notice in the Description page of Project Settings.

#include "Guild.h"
#include "PressureTriggeredComponent.h"

#define OUT

// Sets default values for this component's properties
UPressureTriggeredComponent::UPressureTriggeredComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPressureTriggeredComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get the door actor this component is attached to
	Owner = GetOwner();

	// Get the initial door angle
	InitialAngle = Owner->GetActorRotation().Yaw;

	// Log error if the pressure plate is missing
	if (!PressurePlate) {
		UE_LOG(LogTemp, Warning, TEXT("%s PressureTriggered component has no pressure plate assigned"), *Owner->GetName())
	}
}

// Gets the mass of all actors on top of the plate in kg
float UPressureTriggeredComponent::GetTotalMassOfActorsOnPlate()
{
	// Bail out if the pressure plate isn't assigned
	if (!PressurePlate) { return 0.0f; }

	// Add up their masses and return
	float TotalMass = 0.0f;

	// Get the actors that are overlapping the plate
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through them and add up their masses
	for (const auto& Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

// Called every frame
void UPressureTriggeredComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the PressuePlate Trigger Volume
	if (GetTotalMassOfActorsOnPlate() >= TriggerMass)
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}

