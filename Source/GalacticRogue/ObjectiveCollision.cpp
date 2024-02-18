#include "ObjectiveCollision.h"
#include "CharacterObjectives.h"

UObjectiveCollision::UObjectiveCollision()
{
    // Bind the overlap event
    OnComponentBeginOverlap.AddDynamic(this, &UObjectiveCollision::OnOverlapBegin);
}

void UObjectiveCollision::BeginPlay()
{
    Super::BeginPlay();
    // Other initialization code if necessary
}

void UObjectiveCollision::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UCharacterObjectives* ObjectivesComponent = Cast<UCharacterObjectives>(OtherActor->GetComponentByClass(UCharacterObjectives::StaticClass()));
    if (ObjectivesComponent)
    {
        // Check if the overlapping actor is the character and if they are holding the object with the specific tag
        if (IsCharacterHoldingTaggedObject(OtherActor, "EndIntel"))
        { 
            AActor* OwnerActor = GetOwner();
            if (OwnerActor)
            {
                ObjectivesComponent->CurrentObjective = ObjectiveOnTrigger;
                OwnerActor->Destroy();
            }
        }
        else if (!IsCharacterHoldingTaggedObject(OtherActor, "EndIntel") && this->ComponentHasTag("beforeEndIntel"))
        {
            // Proceed with other objectives as usual
            AActor* OwnerActor = GetOwner();
            if (OwnerActor)
            {
                ObjectivesComponent->CurrentObjective = ObjectiveOnTrigger;
                OwnerActor->Destroy();
            }
        }
    }
}

bool UObjectiveCollision::IsCharacterHoldingTaggedObject(AActor* CharacterActor, const FName& Tag)
{
    UGrabber* GrabberComponent = CharacterActor->FindComponentByClass<UGrabber>();
    if (GrabberComponent)
    {
        return GrabberComponent->IsHeldItemTagged(Tag);
    }
    return false;
}