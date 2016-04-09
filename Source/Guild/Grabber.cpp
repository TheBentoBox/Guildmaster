// Fill out your copyright notice in the Description page of Project Settings.

#include "Guild.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	/// Get owning actor this instance is attached to
	/// We use the owner a lot so it's better to have an easy reference
	Owner = GetOwner();

	FindAttachedPhysicsHandle();
	SetupInputComponent();
}

/// Finds the assumed attached physics handle component
void UGrabber::FindAttachedPhysicsHandle()
{
	/// Look for attached physics handle component
	PhysicsHandle = Owner->FindComponentByClass<UPhysicsHandleComponent>();

	// No PhysicsHandle found
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber cannot function because owner (%s) has no PhysicsHandle"), *Owner->GetName())
	}
}

// Finds the attached input component which appears at runtime
void UGrabber::SetupInputComponent()
{
	/// Look for attached input component
	InputComponent = Owner->FindComponentByClass<UInputComponent>();

	/// Input component found
	/// We can now bind the input axes to the Grab and Release functions
	if (InputComponent)
	{
		/// Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber cannot function because owner (%s) has no InputComponent"), *Owner->GetName())
	}
}

// Finds first physics body in reach
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Setup query parameters for use withing the line trace below
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, Owner);

	/// Line-trace (AKA ray-cast) out to reach distance
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
		);

	return HitResult;
}

// Ray-casts for objects to grab in front of player within reach
void UGrabber::Grab()
{
	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // gets the mesh component in our case
	auto ActorHit = HitResult.GetActor();

	/// If we hit something, then attach it to the physics handle
	if (ActorHit)
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None, // no bones needed
			HitResult.Location,
			true // allow rotation
			);
	}
}

// Releases the currently grabbed object
void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }

	/// If the physics handle is attached, move the attached object
	/// This will move the grabbed object with us once it's attached
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

// Gets the point at the start of our reach, returned as an FVector
FVector UGrabber::GetReachLineStart()
{
	/// Get player view point
	/// Needed because it will be the starting point of our reach line
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
		);

	return PlayerViewPointLocation;
}

// Gets the point at the end of our reach, returned as an FVector
FVector UGrabber::GetReachLineEnd()
{
	/// Get player view point
	/// It is the start of the reach line, so it's necessary to calculate the end
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
		);

	return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
}
