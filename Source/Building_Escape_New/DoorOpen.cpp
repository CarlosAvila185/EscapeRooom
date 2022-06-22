// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorOpen.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"
#define OUT

// Sets default values for this component's properties
UDoorOpen::UDoorOpen()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UDoorOpen::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetOpenYaw = InitialYaw + OpenAngle;
	TargetCloseYaw = TargetOpenYaw - OpenAngle;
	bDoorClosing = false;
	FindDoorAudio();
	FindPressurePlate();
	
}


// Called every frame
void UDoorOpen::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Clock = GetWorld()->GetTimeSeconds();
	
	if (TotalMassOfActors() > MassRequiredToOpen)
	{
		
		OpenDoor(DeltaTime);
		DoorLastOpen = Clock;
	}
	else
	{
		if (Clock - DoorLastOpen > DoorDelayTime)
		{
			
			CloseDoor(DeltaTime);
		}
			
	}
}

void UDoorOpen::FindDoorAudio()
{
	DoorAudio = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!DoorAudio) 
	{
		UE_LOG(LogTemp, Error, TEXT("The object %s is missing an Audio Component"), *GetOwner()->GetName());
	}
}

void UDoorOpen::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("There is no pressure plate assigned to: %s, for the door component!"), *GetOwner()->GetName());
	}
}

void UDoorOpen::OpenDoor(float DeltaTime)
{
	FRotator Rotation = GetOwner()->GetActorRotation();
	Rotation.Yaw = FMath::Lerp(Rotation.Yaw, TargetOpenYaw, DeltaTime * DoorOpeningRate);
	GetOwner()->SetActorRotation(Rotation);
	if (!DoorAudio) { return; }
	if (bDoorOpenning)
	{
		DoorAudio->Play();
	}
	bDoorOpenning = false;
	bDoorClosing = true;
}

void UDoorOpen::CloseDoor(float DeltaTime)
{
	FRotator ClosingRotation = GetOwner()->GetActorRotation();
	ClosingRotation.Yaw = FMath::Lerp(ClosingRotation.Yaw, TargetCloseYaw, DeltaTime * DoorClosingRate);
	GetOwner()->SetActorRotation(ClosingRotation);
	if (!DoorAudio) { return; }
	if (bDoorClosing)
	{
		DoorAudio->Play();
	}
	bDoorClosing = false;
	bDoorOpenning = true;
}

float UDoorOpen::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	//Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	if (OverlappingActors.Num() == 0) { return TotalMass; }
	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->CalculateMass();
	}

	//Sum their masses
	return TotalMass;
}