// Fill out your copyright notice in the Description page of Project Settings.


#include "AmbientSoundComponent.h"

// Sets default values for this component's properties
UAmbientSoundComponent::UAmbientSoundComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    // Initialize the audio component but don't create it here since it's a UActorComponent
    AudioComponent = nullptr;
}

void UAmbientSoundComponent::BeginPlay()
{
    Super::BeginPlay();

    // Initialize and attach the audio component
    AudioComponent = NewObject<UAudioComponent>(this, TEXT("AmbientAudioComponent"));
    if (AudioComponent)
    {
        AudioComponent->RegisterComponent();
        AudioComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
        if (AmbientSoundCue)
        {
            AudioComponent->SetSound(AmbientSoundCue);
        }
    }

    TArray<UBoxComponent*> BoxComponents;
    GetOwner()->GetComponents<UBoxComponent>(BoxComponents);

    for (UBoxComponent* BoxComp : BoxComponents)
    {
        if (BoxComp->ComponentHasTag("ShouldTriggerPlaySound"))
        {
            BoxComp->OnComponentBeginOverlap.AddDynamic(this, &UAmbientSoundComponent::HandleEntranceOverlap);
        }
        else if (BoxComp->ComponentHasTag("StopSoundTag"))
        {
            BoxComp->OnComponentEndOverlap.AddDynamic(this, &UAmbientSoundComponent::HandleExitOverlap);
        }
    }
}


void UAmbientSoundComponent::HandleEntranceOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Check if the other actor is the player
    ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
    if (PlayerCharacter && PlayerCharacter->IsPlayerControlled())
    {
        if (bShouldTriggerPlaySound)
        {
            if (AudioComponent && !AudioComponent->IsPlaying())
            {
                AudioComponent->Play();
            }
        }
        else
        {
            if (AudioComponent && AudioComponent->IsPlaying())
            {
                AudioComponent->Stop();
            }
        }
    }
}

void UAmbientSoundComponent::HandleExitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
    if (PlayerCharacter && PlayerCharacter->IsPlayerControlled())
    {
        if (AudioComponent && AudioComponent->IsPlaying())
        {
            AudioComponent->Stop();
        }
    }
}

