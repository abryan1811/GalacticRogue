#include "ButtonInteractionComponent.h"
#include "Mover.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include <Kismet/GameplayStatics.h>

UButtonInteractionComponent::UButtonInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UButtonInteractionComponent::BeginPlay()
{
    Super::BeginPlay();
    PointLight = Cast<UPointLightComponent>(GetOwner()->GetComponentByClass(UPointLightComponent::StaticClass()));

    // Initialize light color
    UpdateLightColor();

    FindMoversToActivate();
}

void UButtonInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Continuously check the generator power level
    CheckGeneratorPowerLevel();
}

void UButtonInteractionComponent::OnInteract() {
    UE_LOG(LogTemp, Warning, TEXT("OnInteract called."));
    if (bIsOn) {
        UE_LOG(LogTemp, Warning, TEXT("Button is on."));
        if (bIsOn && !isRed) {
            for (UMover* Mover : MoversToActivate) {
                if (Mover) {
                    Mover->SetShouldMove(true); // Activate the mover
                    UE_LOG(LogTemp, Warning, TEXT("Activating Mover: %s"), *Mover->GetName());
                }
            }
            isRed = true; // Set the flag when interacted
            UpdateLightColor(); // Call this to update the light color
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("Button is already red. No further action."));
        }
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Interaction not allowed. Button is inactive."));
    }
}


void UButtonInteractionComponent::CheckGeneratorPowerLevel()
{
    TArray<AActor*> FoundGenerators;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), MatchingGeneratorTag, FoundGenerators);

    if (FoundGenerators.Num() > 0) {
        for (AActor* Generator : FoundGenerators) {
            if (Generator->ActorHasTag("powerlevel2")) {
                bIsOn = true;
                UpdateLightColor();
                return; // Exit as soon as a generator on power level 2 is found
            }
        }
    }
    bIsOn = false;
    UpdateLightColor(); // Update light color if no generator on power level 2 is found
}

void UButtonInteractionComponent::UpdateLightColor() {
    if (PointLight) {
        if (isRed && bIsOn) {
            PointLight->SetIntensity(TargetIntensity);
            PointLight->SetLightColor(FLinearColor::Red);
        } else if (bIsOn) {
            PointLight->SetIntensity(TargetIntensity);
            PointLight->SetLightColor(FLinearColor::Blue);
        } else {
            PointLight->SetIntensity(0.0f);
            PointLight->SetLightColor(FLinearColor::Black);
        }
    }
}

void UButtonInteractionComponent::FindMoversToActivate()
{
    TArray<AActor*> ActorsWithTag;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), MoverTag, ActorsWithTag);

    for (AActor* Actor : ActorsWithTag)
    {
        UMover* MoverComponent = Cast<UMover>(Actor->GetComponentByClass(UMover::StaticClass()));
        if (MoverComponent)
        {
            MoversToActivate.Add(MoverComponent);
        }
    }
}

