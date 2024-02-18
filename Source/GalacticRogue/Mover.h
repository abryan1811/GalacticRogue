// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundCue.h"
#include "Components/ActorComponent.h"
#include "Mover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GALACTICROGUE_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMover();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetShouldMove(bool ShouldMove);


	UPROPERTY(EditAnywhere)
		USoundCue* SlidingDoorSoundCue;

private:

	UPROPERTY(EditAnywhere)
		FRotator RotationOffset;

	UPROPERTY(EditAnywhere)
		float RotationTime = 4;

	FRotator OriginalRotation;

	UPROPERTY(EditAnywhere)
		FVector MoveOffset;

	UPROPERTY(EditAnywhere)
		float MoveTime = 4;

	UPROPERTY(EditAnywhere)
		bool ShouldMove = false;

	UPROPERTY()
		bool bIsCurrentlyMoving;

	FVector OriginalLocation;
		
};