// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootstepComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GALACTICROGUE_API UFootstepComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFootstepComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    // A timer handle for the footstep sound timing
    FTimerHandle FootstepTimerHandle;

private:
    bool bIsLeftFoot = true;

    // Function to play the footstep sound
    void PlayFootstepSound();

    UPROPERTY(EditAnywhere, Category = "Sound")
        USoundBase* LeftFootstepSound;

    UPROPERTY(EditAnywhere, Category = "Sound")
        USoundBase* RightFootstepSound;

    // The interval between footsteps
    UPROPERTY(EditAnywhere, Category = "Sound")
        float FootstepInterval = 0.5f; // Adjust this to match your character's walk cycle
};
