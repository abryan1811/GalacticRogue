#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "TriggerTwoOnGenerator.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GALACTICROGUE_API UTriggerTwoOnGenerator : public UBoxComponent
{
    GENERATED_BODY()

public:
    UTriggerTwoOnGenerator();

    UPROPERTY(EditAnywhere, Category = "Objectives")
        FString ObjectiveForPowerLevel2;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UMover* MoverComponent;

    void OpenPowerLevel1Door();

    AActor* GetAcceptableActor() const;

    void UpdateObjectiveAfterDoorOpens();

    void UpdateObjectiveString(const FString& NewObjective);

    UPROPERTY(EditAnywhere)
        FName PuzzleGeneratorActorTag;

    UPROPERTY(EditAnywhere)
        FVector DesiredLocation;

    UPROPERTY(EditAnywhere)
        FRotator DesiredRotation;

    UPROPERTY(EditAnywhere, Category = "Trigger Settings")
        FName DoorTagToOpen = "PowerLevel1Door";

    UPROPERTY(EditAnywhere, Category = "Audio")
        USoundCue* LowPowerGeneratorCue;

    UPROPERTY(EditAnywhere, Category = "Audio")
        USoundCue* FullPowerGeneratorCue;

private:

    bool bIsPlayingLowPowerCue = false;
    bool bIsPlayingFullPowerCue = false;
};
