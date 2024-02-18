#include "TriggerOneOnGenerator.h"
#include "Mover.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "CharacterObjectives.h"

UTriggerOneOnGenerator::UTriggerOneOnGenerator()
{
    PrimaryComponentTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material1Finder(TEXT("MaterialInstanceConstant'/Game/SciFiCorridors/Materials/MI_generator4.MI_generator4'"));
    MaterialPowerLevel1 = Material1Finder.Object;

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material2Finder(TEXT("MaterialInstanceConstant'/Game/SciFiCorridors/Materials/MI_generator3.MI_generator3'"));
    MaterialPowerLevel2 = Material2Finder.Object;
}

void UTriggerOneOnGenerator::BeginPlay()
{
    Super::BeginPlay();
}

void UTriggerOneOnGenerator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor* ParentActor = GetOwner();
    AActor* TriggeringActor = GetAcceptableActor();

    if (TriggeringActor && !TriggeringActor->IsAttachedTo(ParentActor))
    {
        if (TriggeringActor->ActorHasTag("grabbable"))
        {
            TriggeringActor->Tags.Remove("grabbable");
            UE_LOG(LogTemp, Warning, TEXT("%s is no longer grabbable"), *TriggeringActor->GetName());
        }

        if (ParentActor->ActorHasTag("powerlevel1"))
        {
            FString NewObjectiveForLevel2 = ObjectiveForPowerLevel2; // Customize this string
            UpdateObjectiveString(NewObjectiveForLevel2);
            ParentActor->Tags.Remove("powerlevel1");
            ParentActor->Tags.Add("powerlevel2");
            UE_LOG(LogTemp, Warning, TEXT("%s power level updated to 2"), *ParentActor->GetName());

            // Play the full power sound cue
            if (FullPowerGeneratorCue && !bIsPlayingFullPowerCue)
            {
                bIsPlayingFullPowerCue = true;
                bIsPlayingLowPowerCue = false; // Ensure to stop playing low power cue
                UGameplayStatics::SpawnSoundAttached(FullPowerGeneratorCue, ParentActor->GetRootComponent());
            }
        }
        else if (!ParentActor->ActorHasTag("powerlevel2"))
        {
            ParentActor->Tags.Add("powerlevel1");
            UE_LOG(LogTemp, Warning, TEXT("%s power level set to 1"), *ParentActor->GetName());
            OpenPowerLevel1Door();

            // Play the low power sound cue
            if (LowPowerGeneratorCue && !bIsPlayingLowPowerCue)
            {
                bIsPlayingLowPowerCue = true;
                bIsPlayingFullPowerCue = false; // Ensure to stop playing full power cue
                UGameplayStatics::SpawnSoundAttached(LowPowerGeneratorCue, ParentActor->GetRootComponent());
            }
        }

        UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(TriggeringActor->GetRootComponent());
        if (PrimitiveComponent)
        {
            PrimitiveComponent->SetSimulatePhysics(false);
        }

        TriggeringActor->SetActorLocationAndRotation(DesiredLocation, DesiredRotation);
        TriggeringActor->AttachToComponent(ParentActor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
    }

    UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(ParentActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
    if (MeshComponent)
    {
        if (ParentActor->ActorHasTag("powerlevel1") && MaterialPowerLevel1)
        {
            MeshComponent->SetMaterial(0, MaterialPowerLevel1);
        }
        else if (ParentActor->ActorHasTag("powerlevel2") && MaterialPowerLevel2)
        {
            MeshComponent->SetMaterial(0, MaterialPowerLevel2);
        }
    }
}

void UTriggerOneOnGenerator::OpenPowerLevel1Door()
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

AActor* UTriggerOneOnGenerator::GetAcceptableActor() const
{
    TArray<AActor*> OverlappingActors;
    GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor && Actor->ActorHasTag(PuzzleGeneratorActorTag))
        {
            return Actor;
        }
    }

    return nullptr;
}

void UTriggerOneOnGenerator::UpdateObjectiveAfterDoorOpens()
{
    // Define a new objective string for when the door opens
    FString NewObjective = TEXT("Interesting, a doors opened up.");

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

void UTriggerOneOnGenerator::UpdateObjectiveString(const FString& NewObjective)
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