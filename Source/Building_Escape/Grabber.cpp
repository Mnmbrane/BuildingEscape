// Copyright Manny Candidato 2021

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UGrabber::UGrabber() :
	mReach(100.f),
	mPhysicsHandle(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	// Check Physics handle component
	mPhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if(mPhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not have a PhysicsHandle attached!"), *GetOwner()->GetName())
	}
	else
	{
		// Physics Handle found
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get the players viewpoint
	FVector lPlayerViewPointLocation;
	FRotator lPlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		lPlayerViewPointLocation,	// OUT
		lPlayerViewPointRotation);	// OUT

	// Draw a line from player showing the reach

	FVector lLineTraceEnd = lPlayerViewPointLocation + lPlayerViewPointRotation.Vector() * mReach;

	DrawDebugLine(
		GetWorld(),
		lPlayerViewPointLocation,
		lLineTraceEnd,
		FColor(0, 255, 0),
		false,
		0,
		0,
		2.f
	);

	FHitResult lHit;
	FCollisionQueryParams lTraceParams(FName(TEXT("")), false, GetOwner());
	// Ray-cast out to a certain distance (Reach)
	GetWorld()->LineTraceSingleByObjectType(
		lHit,	// OUT
		lPlayerViewPointLocation,
		lLineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		lTraceParams
	);

	// See what it hits
	AActor* lHitActor = lHit.GetActor();

	if(lHitActor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s hit!"), *lHitActor->GetName());
	}
}

