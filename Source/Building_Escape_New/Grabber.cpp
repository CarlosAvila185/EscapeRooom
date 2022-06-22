// Copyright Carlos Avila 2021

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"


#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	//We are setting the Physics Handle
	SetPhysicsHandle();
	//We set up the controls
	SetUpInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//We are moving the grabbed object(if any)with the player
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetPlayerLineTraceEnd());
	}
}

//Setting the player's Physics Handle so that we can grab objects
void UGrabber::SetPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	//Check if there is PhysicsHandle so that there are no crashes
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("There was no PhysicsHandleComponent found in actor %s"), *GetOwner()->GetName());
	}
	else
	{
		//Physics Handle is found
	}
}

//Setting up the mapping for the controls, for control go to UE>Engine>Input
void UGrabber::SetUpInputComponent() 
{
	//Initializing our InputComponent(Getting the grabber so that we can assign it the controls)
	InputBut = GetOwner()->FindComponentByClass<UInputComponent>();;
	if (InputBut)
	{
		InputBut->BindAction(TEXT("GrabAction"), IE_Pressed, this, &UGrabber::Grab);
		InputBut->BindAction(TEXT("GrabAction"), IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("There was no InputComponent found in actor %s"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	//Assigning the actor we hit with our Ray to a new FHitResult
	FHitResult HitResult = GetFirstPhysicsBodyHandle();
	//Grabbing item if it's within Player's View Location
	if (HitResult.GetActor())
	{
		if (!PhysicsHandle) {return;}
		PhysicsHandle->GrabComponentAtLocation(
			HitResult.GetComponent(),
			NAME_None,
			GetPlayerWorldPosition()
		);
	}	
}

void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

//We get the player's view point Location so that we can check if object is within range
FVector UGrabber::GetPlayerWorldPosition() const
{
	FVector PlayerLocation;
	FRotator PlayerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
	return PlayerLocation;
}

//We get the player's reach.
FVector UGrabber::GetPlayerLineTraceEnd() const
{
	FVector PlayerLocation;
	FRotator PlayerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
	return GetPlayerWorldPosition() + PlayerRotation.Vector() * Reach;
}

//We are retriving the first item hit by our raytracing
FHitResult UGrabber::GetFirstPhysicsBodyHandle() const
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	//We set up the object that we hit with our line tracing (if any) by using out param Hit
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerWorldPosition(),
		GetPlayerLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	//Sets the hit actor to a proper AActor
	AActor* ActorHit = Hit.GetActor();

	return Hit;
}