#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "EndGoalTrigger.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GALACTICROGUE_API UEndGoalTrigger : public UBoxComponent
{
    GENERATED_BODY()

public:
    UEndGoalTrigger();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
        TSubclassOf<UUserWidget> ObjectiveWidgetClass;

    UPROPERTY()
    UUserWidget* ObjectiveWidgetInstance;

    UFUNCTION(BlueprintImplementableEvent, Category = "Cutscene")
        void HideObjectiveWidget();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    void StartCutscene();
    void MoveUpAndDestroy(AActor* ActorToMove);
    void HandleSecondShip();
    void EndCutscene();

private:
    // Timer handle for controlling the movement of the first ship
    FTimerHandle MovementTimer;

    // Timer handle for controlling the movement of the second ship
    FTimerHandle SecondShipMovementTimer;

    // Tag to check for the specific actor that should trigger the cutscene
    UPROPERTY(EditAnywhere, Category = "Cutscene")
        FName SpecificActorTag;

    // Animation montage for startup ship if needed
    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* StartupShipMontage;

    // Sound cues for moving up and handling the second ship
    UPROPERTY(EditAnywhere, Category = "Audio")
        USoundCue* MovingSoundCue;

    UPROPERTY(EditAnywhere, Category = "Audio")
        USoundCue* HandleSecondShipSoundCue;

    // Audio components to control playback of sound cues
    UPROPERTY(Transient)
        UAudioComponent* MovingSoundComponent;

    UPROPERTY(Transient)
        UAudioComponent* SecondShipSoundComponent;
};
