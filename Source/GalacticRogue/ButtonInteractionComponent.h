#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/PointLightComponent.h"
#include "Mover.h"
#include "ButtonInteractionComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GALACTICROGUE_API UButtonInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UButtonInteractionComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY(EditAnywhere, Category = "Setup")
        FName MoverTag;

    UPROPERTY(VisibleAnywhere)
        UPointLightComponent* PointLight;

    UPROPERTY(EditAnywhere)
        float TargetIntensity = 10000.0f;

    TArray<UMover*> MoversToActivate;
    bool isRed = false; // New flag to track red light state

    void FindMoversToActivate();
    void CheckGeneratorPowerLevel();
    void UpdateLightColor();

public:
    UFUNCTION(BlueprintCallable)
        void OnInteract();

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Button")
        bool bIsOn;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button")
        FName MatchingGeneratorTag;
};
