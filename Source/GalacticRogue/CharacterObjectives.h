#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterObjectives.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GALACTICROGUE_API UCharacterObjectives : public UActorComponent
{
    GENERATED_BODY()

public:
    UCharacterObjectives();
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Objectives")
        FString CurrentObjective;

    UFUNCTION(BlueprintCallable, Category = "Objectives")
        void UpdateCurrentObjective(const FString& NewObjective);

protected:
    virtual void BeginPlay() override;

    // Make sure this is public or has a getter function if you need to access it from Blueprints
   
};
