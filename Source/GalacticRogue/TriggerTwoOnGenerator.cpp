#include "TriggerTwoOnGenerator.h"
#include "Mover.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/PrimitiveComponent.h"
#include "CharacterObjectives.h"

UTriggerTwoOnGenerator::UTriggerTwoOnGenerator()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerTwoOnGenerator::BeginPlay()
{
    Super::BeginPlay();
    // Additional initialization logic can go here if needed
}

void UTriggerTwoOnGenerator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor* GeneratorActor = GetOwner();
    AActor* TriggeringActor = GetAcceptableActor();

    if (TriggeringActor && !TriggeringActor->IsAttachedTo(GeneratorActor))
    {
        if (TriggeringActor->ActorHasTag("grabbable"))
        {
            TriggeringActor->Tags.Remove("grabbable");
            UE_LOG(LogTemp, Warning, TEXT("%s is no longer grabbable"), *TriggeringActor->GetName());
        }

        if (GeneratorActor->ActorHasTag("powerlevel1"))
        {
            FString NewObjectiveForLevel2 = ObjectiveForPowerLevel2; // Customize this string
            UpdateObjectiveString(NewObjectiveForLevel2);

            GeneratorActor->Tags.Remove("powerlevel1");
            GeneratorActor->Tags.Add("powerlevel2");
            UE_LOG(LogTemp, Warning, TEXT("Generator %s power level updated to 2"), *GeneratorActor->GetName());

            // Play the full power sound cue
            if (FullPowerGeneratorCue && !bIsPlayingFullPowerCue)
            {
                bIsPlayingFullPowerCue = true;
                bIsPlayingLowPowerCue = false; // Ensure to stop playing low power cue
                UGameplayStatics::SpawnSoundAttached(FullPowerGeneratorCue, GeneratorActor->GetRootComponent());
            }
        }
        else if (!GeneratorActor->ActorHasTag("powerlevel2"))
        {
            GeneratorActor->Tags.Add("powerlevel1");
            UE_LOG(LogTemp, Warning, TEXT("Generator %s power level set to 1"), *GeneratorActor->GetName());
            OpenPowerLevel1Door(); // Open the door when power level 1 is reached

            // Play the low power sound cue
            if (LowPowerGeneratorCue && !bIsPlayingLowPowerCue)
            {
                bIsPlayingLowPowerCue = true;
                bIsPlayingFullPowerCue = false; // Ensure to stop playing full power cue
                UGameplayStatics::SpawnSoundAttached(LowPowerGeneratorCue, GeneratorActor->GetRootComponent());
            }
        }

        UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(TriggeringActor->GetRootComponent());
        if (PrimitiveComponent)
        {
            PrimitiveComponent->SetSimulatePhysics(false);
        }

        TriggeringActor->SetActorLocationAndRotation(DesiredLocation, DesiredRotation);
        TriggeringActor->AttachToComponent(GeneratorActor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
    }
}

void UTriggerTwoOnGenerator::OpenPowerLevel1Door()
{
    TArray<AActor*> ActorsWithTag;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), DoorTagToOpen, ActorsWithTag);

    for (AActor* Actor : ActorsWithTag)
    {
        UMover* MoverComp = Actor->FindComponentByClass<UMover>();
        if (MoverComp)
        {
            MoverComp->SetShouldMove(true);
            UE_LOG(LogTemp, Warning, TEXT("Door with Tag %s is now moving."), *DoorTagToOpen.ToString());

            UpdateObjectiveAfterDoorOpens();
        }
    }
}

AActor* UTriggerTwoOnGenerator::GetAcceptableActor() const
{
    TArray<AActor*> OverlappingActors;
    GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor && Actor->ActorHasTag(this->PuzzleGeneratorActorTag))
        {
            return Actor;
        }
    }

    return nullptr;
}

void UTriggerTwoOnGenerator::UpdateObjectiveAfterDoorOpens()
{
    // Define a new objective string for when the door opens
    FString NewObjective = TEXT("Oh interesting, a doors opened in here.");

    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (PlayerCharacter)
    {
        UCharacterObjectives* ObjectivesComponent = Cast<UCharacterObjectives>(PlayerCharacter->GetComponentByClass(UCharacterObjectives::StaticClass()));
        if (ObjectivesComponent)
        {
            ObjectivesComponent->UpdateCurrentObjective(NewObjective);
        }
    }
}

void UTriggerTwoOnGenerator::UpdateObjectiveString(const FString& NewObjective)
{
    // Get the player character's UCharacterObjectives component to update its CurrentObjective
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (PlayerCharacter)
    {
        UCharacterObjectives* ObjectivesComponent = Cast<UCharacterObjectives>(PlayerCharacter->GetComponentByClass(UCharacterObjectives::StaticClass()));
        if (ObjectivesComponent)
        {
            ObjectivesComponent->UpdateCurrentObjective(NewObjective);
        }
    }
}