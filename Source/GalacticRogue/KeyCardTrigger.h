#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include <Components/PointLightComponent.h>
#include "KeyCardTrigger.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GALACTICROGUE_API UKeyCardTrigger : public UBoxComponent
{
    GENERATED_BODY()

public:
    UKeyCardTrigger();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // This function is used to start the movement of the keycard.
    void StartMovingKeycard(AActor* KeyCard);

    // This function is used to actually move the keycard.
    void MoveKeycard(AActor* KeyCardActor, float DeltaTime);

    void ActivateMatchingMover();
    
    void OnMovementComplete(); 
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeyCard Trigger")
        UPointLightComponent* PointLight;

private:
    FTimerHandle TimerHandle;
    
    UPROPERTY(EditAnywhere, Category = "KeyCard Trigger")
        FName KeyCardTag;

    UPROPERTY(EditAnywhere, Category = "KeyCard Trigger")
        FName MoverTag;

    UPROPERTY(EditAnywhere, Category = "Animation")
        FVector DesiredInitialLocation;

    UPROPERTY(EditAnywhere, Category = "Animation")
        FVector DesiredEndLocation;

    UPROPERTY(EditAnywhere, Category = "Animation")
        float MovementDuration = 2.0f;

    bool bIsKeycardMoving = false;
    bool shouldMove = false;
    float MovementElapsedTime = 0.0f;

    // A reference to the keycard actor that will be set when the overlap begins.
    AActor* KeyCardActor = nullptr;
};
