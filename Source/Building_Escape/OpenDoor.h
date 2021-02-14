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
	void FindAudioComponent();

	UPROPERTY(EditAnywhere)
	float mDoorCloseDelay;

	UPROPERTY(EditAnywhere)
	float mTargetYaw;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* mPressurePlate;

	UPROPERTY(EditAnywhere)
	float mOpenDoorInterpSpeed;

	UPROPERTY(EditAnywhere)
	float mCloseDoorInterpSpeed;

	UPROPERTY(EditAnywhere)
	float mMassToOpenDoor;

	UPROPERTY()
	UAudioComponent* mAudioComponent;

	float mInitialYaw;
	float mCurrentYaw;

	float mDoorLastOpened;

	bool mIsOpening;
};
