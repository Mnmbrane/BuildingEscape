// Copyright Manny Candidato 2021


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor() :
	mDoorCloseDelay(.5f),
	mTargetYaw(90.f),
	mPressurePlate(nullptr),
	mOpenDoorInterpSpeed(60.f),
	mCloseDoorInterpSpeed(60.f),
	mMassToOpenDoor(50.f),
	mAudioComponent(nullptr),
	mInitialYaw(0.f),
	mCurrentYaw(0.f),
	mDoorLastOpened(0.f),
	mIsOpening(false)
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

	FindAudioComponent();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Early return when pressure plate is null
	if(mPressurePlate != nullptr)
	{
		if(TotalMassOfActor() >= mMassToOpenDoor)
		{
			OpenDoor(DeltaTime);
			mDoorLastOpened = GetWorld()->GetTimeSeconds();
		}
		else
		{
			if(GetWorld()->GetTimeSeconds() - mDoorLastOpened >= mDoorCloseDelay)
			{
				CloseDoor(DeltaTime);
			}
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	mCurrentYaw = GetOwner()->GetActorRotation().Yaw;
	FRotator lDoorRotation = GetOwner()->GetActorRotation();

	lDoorRotation.Yaw = FMath::FInterpConstantTo(mCurrentYaw, mTargetYaw, DeltaTime, mOpenDoorInterpSpeed);

	GetOwner()->SetActorRotation(lDoorRotation);

	if(mIsOpening == false)
	{
		mAudioComponent->Play();
		mIsOpening = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	mCurrentYaw = GetOwner()->GetActorRotation().Yaw;
	FRotator lDoorRotation = GetOwner()->GetActorRotation();

	lDoorRotation.Yaw = FMath::FInterpConstantTo(mCurrentYaw, mInitialYaw, DeltaTime, mCloseDoorInterpSpeed);

	GetOwner()->SetActorRotation(lDoorRotation);

	if(mIsOpening == true)
	{
		mAudioComponent->Play();
		mIsOpening = false;
	}
}

float UOpenDoor::TotalMassOfActor() const
{
	float lTotalMass = 0.f;

	if(mPressurePlate != nullptr)
	{
		// Find all overlapping Actors
		TArray<AActor*> lOverlappingActors;
		mPressurePlate->GetOverlappingActors(
			lOverlappingActors // OUT
			);
		// Add up their masses
		for(AActor* actor : lOverlappingActors)
		{
			lTotalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
	}

	return lTotalMass;
}

void UOpenDoor::FindAudioComponent()
{
	mAudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if(mAudioComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not have an Audio Component attached!"), *GetOwner()->GetName())
	}
}