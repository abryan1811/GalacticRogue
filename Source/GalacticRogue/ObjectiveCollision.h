#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Grabber.h"
#include "ObjectiveCollision.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GALACTICROGUE_API UObjectiveCollision : public UBoxComponent
{
    GENERATED_BODY()
public:
    UObjectiveCollision();

    UPROPERTY(EditAnywhere, Category = "Objective")
        FString ObjectiveOnTrigger;

    bool IsCharacterHoldingTaggedObject(AActor* CharacterActor, const FName& Tag);

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

