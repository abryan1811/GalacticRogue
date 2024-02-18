#include "EndGoalTrigger.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerController.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

UEndGoalTrigger::UEndGoalTrigger()
{
    OnComponentBeginOverlap.AddDynamic(this, &UEndGoalTrigger::OnOverlapBegin);
}

void UEndGoalTrigger::BeginPlay()
{
    Super::BeginPlay();

    // Get the Skeletal Mesh Component, which is assumed to be named 'Belica'
    USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));

    if (SkeletalMeshComponent)
    {
        // Initially prevent the Skeletal Mesh Component from updating.
        SkeletalMeshComponent->bNoSkeletonUpdate = true;
    }
}

void UEndGoalTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag(SpecificActorTag))
    {
        StartCutscene();
        OtherActor->Destroy();
    }
}

void UEndGoalTrigger::StartCutscene()
{
    // Hide all widgets derived from UUserWidget.
    TArray<UUserWidget*> Widgets;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Widgets, UUserWidget::StaticClass(), false);
    for (UUserWidget* Widget : Widgets)
    {
        if (Widget)
        {
            Widget->RemoveFromParent();
        }
    }

    // Disable HUD drawing.
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (PlayerController && PlayerController->GetHUD())
    {
        PlayerController->GetHUD()->bShowHUD = false;
    }   
    // Handle the main character visibility and input.
    ACharacter* MainCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (MainCharacter)
    {
        MainCharacter->GetMesh()->SetVisibility(false);
        MainCharacter->DisableInput(PlayerController);
    }

    // Switch to the secondary camera.
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsWithTag(this, FName("SecondaryCamera"), FoundActors);
    if (FoundActors.Num() > 0)
    {
        ACameraActor* SecondaryCamera = Cast<ACameraActor>(FoundActors[0]);
        if (SecondaryCamera)
        {
            PlayerController->SetViewTargetWithBlend(SecondaryCamera, 1.0f);
        }
    }

    // Handle the visibility of the 'Belica' mesh component.
    AActor* OwnerActor = GetOwner();
    if (OwnerActor)
    {
        TArray<UActorComponent*> Components;
        OwnerActor->GetComponents(Components);

        for (UActorComponent* Component : Components)
        {
            if (Component->GetName().Equals("Belica", ESearchCase::IgnoreCase))
            {
                USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>(Component);
                if (SkeletalMeshComponent)
                {
                    SkeletalMeshComponent->SetVisibility(true);
                    UE_LOG(LogTemp, Warning, TEXT("Belica visibility set to true."));
                }
            }
        }

        // Pass the existing OwnerActor to the MoveUpAndDestroy method.
        MoveUpAndDestroy(OwnerActor);
    }
    
}


void UEndGoalTrigger::MoveUpAndDestroy(AActor* ActorToMove)
{
    // Define how high you want the actor to move up and the duration
    float MoveHeight = 1500.0f; // Change as per your requirement
    float Duration = 10.0f; // Time in seconds to complete the movement

    FVector StartLocation = ActorToMove->GetActorLocation();
    FVector EndLocation = StartLocation + FVector(0, 0, MoveHeight);

    // Play the moving sound effect
    MovingSoundComponent = UGameplayStatics::SpawnSoundAtLocation(this, MovingSoundCue, ActorToMove->GetActorLocation());

    // Use a timer to update the position every frame
    GetWorld()->GetTimerManager().SetTimer(MovementTimer, [this, ActorToMove, StartLocation, EndLocation, Duration]() mutable {
        static float ElapsedTime = 0;
        ElapsedTime += GetWorld()->DeltaTimeSeconds;

        // Calculate new location
        FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, ElapsedTime / Duration);
        ActorToMove->SetActorLocation(NewLocation);

        // Check if the movement is complete
        if (ElapsedTime >= Duration)
        {
            // Stop the moving sound effect
            if (this->MovingSoundComponent && !this->MovingSoundComponent->IsBeingDestroyed())
            {
                this->MovingSoundComponent->Stop();
            }

            // Stop the timer and destroy the actor
            GetWorld()->GetTimerManager().ClearTimer(MovementTimer);
            ActorToMove->Destroy();
            ElapsedTime = 0; // Reset for next time

            this->HandleSecondShip();
        }
        }, GetWorld()->DeltaTimeSeconds, true);
}

void UEndGoalTrigger::HandleSecondShip()
{
    TArray<AActor*> FoundShips;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SecondShip"), FoundShips);

    if (FoundShips.Num() > 0)
    {
        AActor* SecondShip = FoundShips[0];

        // Play the sound effect for the second ship
        UGameplayStatics::PlaySoundAtLocation(this, HandleSecondShipSoundCue, SecondShip->GetActorLocation());

        // Iterate through all components and make mesh components visible
        TArray<UActorComponent*> Components;
        SecondShip->GetComponents(Components);
        for (UActorComponent* Component : Components)
        {
            UMeshComponent* MeshComponent = Cast<UMeshComponent>(Component);
            if (MeshComponent)
            {
                MeshComponent->SetVisibility(true);
            }
        }

        // Start location is the ship's current location
        FVector StartLocation = SecondShip->GetActorLocation();

        // End location is the desired endpoint from the provided coordinates
        FVector EndLocation(-3528.128563f, -7713.284675f, 7475.558604f);

        float MoveDuration = 5.0f; // Duration for the ship to move to the end location

        // Set up a timer to move the ship to the end location
        GetWorld()->GetTimerManager().SetTimer(SecondShipMovementTimer, [this, SecondShip, StartLocation, EndLocation, MoveDuration]() mutable {
            static float ElapsedTime = 0;
            ElapsedTime += GetWorld()->DeltaTimeSeconds;

            FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, ElapsedTime / MoveDuration);
            SecondShip->SetActorLocation(NewLocation);

            // Once the elapsed time reaches the duration, stop the sound and destroy the ship
            if (ElapsedTime >= MoveDuration)
            {
                // Stop the sound effect for the second ship
                if (SecondShipSoundComponent && !SecondShipSoundComponent->IsBeingDestroyed())
                {
                    SecondShipSoundComponent->Stop();
                }

                GetWorld()->GetTimerManager().ClearTimer(SecondShipMovementTimer);
                SecondShip->Destroy();
                ElapsedTime = 0; // Reset for next time

                UGameplayStatics::OpenLevel(this, FName("EndGameScreen"));
            }
            }, GetWorld()->DeltaTimeSeconds, true);

        UE_LOG(LogTemp, Warning, TEXT("SecondShip is now moving towards the end location."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SecondShip not found"));
    }
}

void UEndGoalTrigger::EndCutscene()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (PlayerController)
    {
        // Show all widgets by adding them back to the viewport
        // Assuming WidgetsToShow is a TArray of UUserWidget* that you want to show again
        // Add logic to re-add widgets to the viewport
    }
}