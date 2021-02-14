// Copyright Manny Candidato 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActor() const;


	float mInitialYaw;
	float mCurrentYaw;

	float mDoorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
	float mDoorCloseDelay = .5f;

	UPROPERTY(EditAnywhere)
	float mTargetYaw = 90.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* mPressurePlate;

	UPROPERTY(EditAnywhere)
	AActor* mActorThatOpens;

	UPROPERTY(EditAnywhere)
	float mOpenDoorInterpSpeed = 60.f;

	UPROPERTY(EditAnywhere)
	float mCloseDoorInterpSpeed = 60.f;

	UPROPERTY(EditAnywhere)
	float mMassToOpenDoor = 50.f;
};
