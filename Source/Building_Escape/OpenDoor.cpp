// Copyright Manny Candidato 2021


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	mInitialYaw = GetOwner()->GetActorRotation().Yaw;
	mCurrentYaw = mInitialYaw;
	
	mTargetYaw += mInitialYaw;

	if(mPressurePlate == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has the open door component but no pressure plate set"), *GetOwner()->GetName())
	}

	mActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(mPressurePlate != nullptr && mPressurePlate->IsOverlappingActor(mActorThatOpens))
	{
		OpenDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	mCurrentYaw = GetOwner()->GetActorRotation().Yaw;
	FRotator lDoorRotation = GetOwner()->GetActorRotation();

	lDoorRotation.Yaw = FMath::FInterpConstantTo(mCurrentYaw, mTargetYaw, DeltaTime, 30);

	GetOwner()->SetActorRotation(lDoorRotation);
}
