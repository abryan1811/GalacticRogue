#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "HeldObjectWidget.h"
#include "UIObjectPreviewActor.h"
#include "Grabber.generated.h"

// Forward declarations
class USphereComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GALACTICROGUE_API UGrabber : public USceneComponent
{
    GENERATED_BODY()

public:
    UGrabber();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable)
        void Grabbed();

    UFUNCTION(BlueprintCallable)
        void Release();

    void DisplayPickupMessage();

    bool IsHolding() const;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grabber")
        float GrabRadius = 100;

    UFUNCTION(BlueprintCallable)
        void InteractWithObject();
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grabber")
        float HeadHeight = 180.0f; // Example value, adjust to match your character's scale

    // How far in front of the character the line trace should start
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grabber")
        float StartOffset = 100.0f; // Example value, adjust as needed

    UPROPERTY(EditDefaultsOnly, Category = "UI")
        TSubclassOf<UUserWidget> HeldObjectWidgetClass;

    bool IsHeldItemTagged(const FName& Tag) const;

private:

    UPROPERTY(EditAnywhere)
        float Reach = 100.0f;
    // Held item actor
    AActor* HeldItem;
    // Is an item being held?
    bool bIsHolding;
    // Component used for detecting overlaps with grabbable items
    USphereComponent* CollisionComponent;
    // The vertical offset from the character's feet to their head
    UPROPERTY()
        UUserWidget* HeldObjectWidgetInstance;
    
    UPROPERTY()
    UHeldObjectWidget* HeldObjectUI;
};