#include "KeyCardTrigger.h"
#include "Mover.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

UKeyCardTrigger::UKeyCardTrigger()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void UKeyCardTrigger::BeginPlay()
{
    Super::BeginPlay();
    OnComponentBeginOverlap.AddDynamic(this, &UKeyCardTrigger::OnOverlapBegin);

    // Initialize the PointLight reference by finding the component
    PointLight = GetOwner()->FindComponentByClass<UPointLightComponent>();
    if (!PointLight)
    {
        UE_LOG(LogTemp, Error, TEXT("PointLight component not found on %s"), *GetOwner()->GetName());
    }
}

void UKeyCardTrigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsKeycardMoving && shouldMove)
    {
        MoveKeycard(KeyCardActor, DeltaTime); // Pass DeltaTime here
    }

}

void UKeyCardTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag(KeyCardTag))
    {
        UE_LOG(LogTemp, Warning, TEXT("KeyCard with Tag %s has triggered overlap."), *KeyCardTag.ToString());
        StartMovingKeycard(OtherActor); // Start moving the keycard
    }
}

void UKeyCardTrigger::ActivateMatchingMover()
{
    TArray<AActor*> ActorsWithMover;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), ActorsWithMover);

    for (AActor* Actor : ActorsWithMover)
    {
        UMover* MoverComp = Actor->FindComponentByClass<UMover>();
        if (MoverComp && Actor->ActorHasTag(MoverTag))
        {
            MoverComp->SetShouldMove(true);
            UE_LOG(LogTemp, Warning, TEXT("Door with Tag %s is now moving."), *MoverTag.ToString());
        }
    }
}

void UKeyCardTrigger::StartMovingKeycard(AActor* KeyCard)
{
    if (KeyCard)
    {
        KeyCardActor = KeyCard; // Store the reference to the keycard actor
        KeyCardActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform); // Attach the keycard to the swiper
        bIsKeycardMoving = true;
        shouldMove = true;
        MovementElapsedTime = 0.0f;
        KeyCardActor->SetActorLocation(DesiredInitialLocation); // Optionally set an initial location
    }
}

void UKeyCardTrigger::MoveKeycard(AActor* KeyCard, float DeltaTime)
{
    if (KeyCard && MovementElapsedTime < MovementDuration)
    {
        MovementElapsedTime += DeltaTime;
        float Alpha = MovementElapsedTime / MovementDuration;
        FVector NewLocation = FMath::LerpStable(DesiredInitialLocation, DesiredEndLocation, Alpha);
        KeyCard->SetActorLocation(NewLocation);

        UE_LOG(LogTemp, Warning, TEXT("Moving KeyCard to New Location: %s"), *NewLocation.ToString());

        if (Alpha >= 1.0f)
        {
            bIsKeycardMoving = false;
            // Directly call OnMovementComplete when the movement is finished
            OnMovementComplete();
        }
    }
}

void UKeyCardTrigger::OnMovementComplete()
{
    if (PointLight)
    {
        PointLight->SetLightColor(FLinearColor(0.0f, 1.0f, 0.0f)); // RGB for green
        UE_LOG(LogTemp, Warning, TEXT("PointLight color set to green."));
    }

    // Check if we have a valid keycard actor
    if (KeyCardActor)
    {
        KeyCardActor->Destroy();
        KeyCardActor = nullptr; // Clear the reference after destruction
        UE_LOG(LogTemp, Warning, TEXT("KeyCard has been destroyed."));
    }


    ActivateMatchingMover();
}
