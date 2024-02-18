#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInterface.h"
#include "Mover.h"
#include "TriggerOneOnGenerator.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GALACTICROGUE_API UTriggerOneOnGenerator : public UBoxComponent
{
    GENERATED_BODY()

public:
    UTriggerOneOnGenerator();
    UPROPERTY(EditAnywhere, Category = "Objectives")
        FString ObjectiveForPowerLevel2;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Function to open the door when power level 1 is reached
    void OpenPowerLevel1Door();

    // Function to get the first acceptable actor that is overlapping
    AActor* GetAcceptableActor() const;

    void UpdateObjectiveAfterDoorOpens();

    void UpdateObjectiveString(const FString& NewObjective);

    UPROPERTY(EditAnywhere)
        FName PuzzleGeneratorActorTag; // The tag to check for the acceptable actor

    UPROPERTY(EditAnywhere)
        FVector DesiredLocation; // Desired location for the actor to be attached

    UPROPERTY(EditAnywhere)
        FRotator DesiredRotation; // Desired rotation for the actor to be attached

    UPROPERTY(EditAnywhere, Category = "Trigger Settings")
        FName DoorTagToOpen = "PowerLevel1Door"; // Tag of the door to be opened

    UPROPERTY(EditAnywhere, Category = "Audio")
        USoundCue* LowPowerGeneratorCue;

    UPROPERTY(EditAnywhere, Category = "Audio")
        USoundCue* FullPowerGeneratorCue;

private:
    UPROPERTY()
        UMaterialInterface* MaterialPowerLevel1; // Material for Power Level 1

    UPROPERTY()
        UMaterialInterface* MaterialPowerLevel2; // Material for Power Level 2

    bool bIsPlayingLowPowerCue = false;
    bool bIsPlayingFullPowerCue = false;
};
