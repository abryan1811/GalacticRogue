#include "FootstepComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UFootstepComponent::UFootstepComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UFootstepComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UFootstepComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        // Check if the character is moving on the ground
        if (OwnerCharacter->GetCharacterMovement()->IsMovingOnGround() && !OwnerCharacter->GetVelocity().IsZero())
        {
            // Ensure we are not already playing the sound repeatedly
            if (!GetWorld()->GetTimerManager().IsTimerActive(FootstepTimerHandle))
            {
                GetWorld()->GetTimerManager().SetTimer(FootstepTimerHandle, this, &UFootstepComponent::PlayFootstepSound, FootstepInterval, true);
            }
        }
        else
        {
            // Stop the timer if the character is not moving
            GetWorld()->GetTimerManager().ClearTimer(FootstepTimerHandle);
        }
    }
}

void UFootstepComponent::PlayFootstepSound()
{
    USoundBase* FootstepToPlay = bIsLeftFoot ? LeftFootstepSound : RightFootstepSound;
    if (FootstepToPlay)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FootstepToPlay, GetOwner()->GetActorLocation());
    }
    bIsLeftFoot = !bIsLeftFoot; // Toggle the boolean to switch feet for the next step
}