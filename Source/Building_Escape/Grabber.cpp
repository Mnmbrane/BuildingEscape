// Copyright Manny Candidato 2021

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UGrabber::UGrabber() :
	mReach(100.f),
	mPhysicsHandle(nullptr),
	mInputComponent(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	FindPhysicsHandleComponent();

	FindAndSetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	// Check Physics handle component
	mPhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(mPhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not have a PhysicsHandle attached!"), *GetOwner()->GetName())
	}
}

void UGrabber::FindAndSetupInputComponent()
{
	// Check Input component
	mInputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(mInputComponent != nullptr)
	{
		mInputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		mInputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not have an InputComponent attached!"), *GetOwner()->GetName())
	}
}

void UGrabber::Grab()
{
	FVector lLineTraceEnd = GetReachVector();

	FHitResult lHitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* lComponentToGrab = lHitResult.GetComponent();
	AActor* lActorHit = lHitResult.GetActor();

	// If we hit something then attach the physics handle
	if(lActorHit != nullptr)
	{
		if(mPhysicsHandle != nullptr)
		{
			mPhysicsHandle->GrabComponentAtLocation(
				lComponentToGrab,
				NAME_None,
				lLineTraceEnd
			);
		}
	}
}

void UGrabber::Release()
{

	if(mPhysicsHandle != nullptr)
	{
		// If the physics handle is attached
		if(mPhysicsHandle->GrabbedComponent != nullptr)
		{
			// Release the object we are holding
			mPhysicsHandle->ReleaseComponent();
		}
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector lLineTraceEnd = GetReachVector();

	if(mPhysicsHandle != nullptr)
	{
		// If the physics handle is attached
		if(mPhysicsHandle->GrabbedComponent != nullptr)
		{
			// Move the object we are holding
			mPhysicsHandle->SetTargetLocation(lLineTraceEnd);
		}
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector lLineTraceEnd = GetReachVector();

	FHitResult lHit;
	FCollisionQueryParams lTraceParams(FName(TEXT("")), false, GetOwner());
	// Ray-cast out to a certain distance (Reach)
	GetWorld()->LineTraceSingleByObjectType(
		lHit,	// OUT
		GetPlayerWorldPos(),
		lLineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		lTraceParams
	);

	return lHit;
}

FVector UGrabber::GetReachVector() const
{
	// Get the players viewpoint
	FVector lPlayerViewPointLocation;
	FRotator lPlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		lPlayerViewPointLocation,	// OUT
		lPlayerViewPointRotation);	// OUT

	return lPlayerViewPointLocation + lPlayerViewPointRotation.Vector() * mReach;
}

FVector UGrabber::GetPlayerWorldPos() const
{
	// Get the players viewpoint
	FVector lPlayerViewPointLocation;
	FRotator lPlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		lPlayerViewPointLocation,	// OUT
		lPlayerViewPointRotation);	// OUT

	return lPlayerViewPointLocation;
}