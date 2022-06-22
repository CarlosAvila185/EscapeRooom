// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h" 
#include "Engine/TriggerVolume.h"
#include "DoorOpen.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_NEW_API UDoorOpen : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorOpen();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindDoorAudio();
	void FindPressurePlate();

private:
	
	bool bOffPlate,bOnPlate;
	float TargetOpenYaw, CurrentYaw, InitialYaw, TargetCloseYaw, Clock;
	float DoorLastOpen = 0.f;

	//Tracks sound of door
	bool bDoorClosing = false;
	bool bDoorOpenning = false;

	UPROPERTY(EditAnywhere)
		float MassRequiredToOpen = 60.f;

	UPROPERTY(EditAnywhere)
		float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere)
		float DoorDelayTime = 2.f;

	UPROPERTY(EditAnywhere)
		float DoorClosingRate = 2.f;

	UPROPERTY(EditAnywhere)
		float DoorOpeningRate = 1.f;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr;	

	UPROPERTY()
		UAudioComponent* DoorAudio;

};
