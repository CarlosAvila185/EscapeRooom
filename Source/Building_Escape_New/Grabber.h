// Copyright Carlos Avila 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_NEW_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//This defines the distance from where we can grab objects
	UPROPERTY(EditAnywhere)
	float Reach = 200.f;
		
	//setting component to nullptr so that it doesn't crash in any case
	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UPROPERTY()
	UInputComponent* InputBut = nullptr;

	void SetPhysicsHandle();
	void SetUpInputComponent();
	void Grab();
	void Release();

	FVector GetPlayerWorldPosition() const;
	FVector GetPlayerLineTraceEnd() const;
	FHitResult GetFirstPhysicsBodyHandle() const;
};
