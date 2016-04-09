// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GUILD_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// How far the pawn can grab objects from in cm
	float Reach = 100.0f;

	// The actor this component is attached to
	AActor* Owner = nullptr;

	/// Owning actor component references
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// Ray-casts and grabs what's in reach
	void Grab();

	// Releases the currently grabbed object
	void Release();

	// Finds attached physics handle, stored in PhysicsHandle variable
	void FindAttachedPhysicsHandle();

	// Setup assumed attached input commponenet
	void SetupInputComponent();

	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	// Returns closest point within our reach
	FVector GetReachLineStart();

	// Returns furthest point within our reach
	FVector GetReachLineEnd();
};