// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "PressureTriggeredComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUILD_API UPressureTriggeredComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPressureTriggeredComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnOpen;

	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnClose;

private:
	// Returns total mass in kilograms
	float GetTotalMassOfActorsOnPlate();

	// The pressure plate that opens the door
	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr;

	// Mass needed on pressure plate for it to open
	UPROPERTY(EditAnywhere)
		float TriggerMass = 50.0f;

	// The angle the door starts off at
	float InitialAngle;

	// The door actor this component is attached to
	AActor* Owner = nullptr;
	
};
