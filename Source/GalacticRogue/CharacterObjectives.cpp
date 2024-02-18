#include "CharacterObjectives.h"

UCharacterObjectives::UCharacterObjectives()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterObjectives::BeginPlay()
{
    Super::BeginPlay();
    CurrentObjective = (TEXT("I must find a way into that Ship"));
}

void UCharacterObjectives::UpdateCurrentObjective(const FString& NewObjective)
{
    CurrentObjective = NewObjective;
}
