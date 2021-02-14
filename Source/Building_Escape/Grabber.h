// Copyright Manny Candidato 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UGrabber : public UActorComponent
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
	void Grab();
	void Release();
	void FindPhysicsHandleComponent();
	void FindAndSetupInputComponent();

	// Return the line trace end
	FVector GetReachVector() const;

	// Return the first Actor within reach with physics body
	FHitResult GetFirstPhysicsBodyInReach() const;

	// Get Player position in the world
	FVector GetPlayerWorldPos() const;

	UPROPERTY()
	UPhysicsHandleComponent* mPhysicsHandle;

	UPROPERTY()
	UInputComponent* mInputComponent;

	float mReach;
};
