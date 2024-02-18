#include "Grabber.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/InputSettings.h"
#include "ButtonInteractionComponent.h"

UGrabber::UGrabber()
{
    PrimaryComponentTick.bCanEverTick = true;
    bIsHolding = false;

    // Create a sphere component for overlap detection
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("GrabSphere"));
    CollisionComponent->InitSphereRadius(GrabRadius);
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    CollisionComponent->SetGenerateOverlapEvents(true);

   static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/GameplayUI.GameplayUI_C")
   );
    HeldObjectWidgetClass = WidgetClassFinder.Class;
}

void UGrabber::BeginPlay()
{
    Super::BeginPlay();
    CollisionComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

    // Create and display the widget for the held object
    if (HeldObjectWidgetClass != nullptr)
    {
        HeldObjectWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), HeldObjectWidgetClass);
        if (HeldObjectWidgetInstance != nullptr)
        {
            HeldObjectWidgetInstance->AddToViewport();
            HeldObjectUI = Cast<UHeldObjectWidget>(HeldObjectWidgetInstance); // Cast the instance to the correct type
        }
    }
}



void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGrabber::Grabbed()
{
    if (bIsHolding) return;

    TArray<AActor*> OverlappingActors;
    CollisionComponent->GetOverlappingActors(OverlappingActors, TSubclassOf<AActor>());
    for (AActor* Actor : OverlappingActors)
    {
        if (Actor->ActorHasTag(FName("grabbable")))
        {
            UPrimitiveComponent* RootComponent = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
            if (RootComponent)
            {
                RootComponent->SetSimulatePhysics(false);
                RootComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
                RootComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

                USkeletalMeshComponent* SkeletalMeshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
                if (SkeletalMeshComponent)
                {
                    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
                    Actor->AttachToComponent(SkeletalMeshComponent, AttachmentRules, FName("hand_rSocket"));
                    HeldItem = Actor;
                    bIsHolding = true;
                    break;
                }

                bIsHolding = true;
                HeldItem = Actor;
                break;
            }
        }
    }
}


void UGrabber::Release()
{
    if (bIsHolding && HeldItem)
    {
        UPrimitiveComponent* RootComponent = Cast<UPrimitiveComponent>(HeldItem->GetRootComponent());
        if (RootComponent)
        {            
            RootComponent->SetSimulatePhysics(true);
            RootComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            RootComponent->SetCollisionResponseToAllChannels(ECR_Block);
        }

        HeldItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        HeldItem = nullptr;
        bIsHolding = false;
    }
}

bool UGrabber::IsHolding() const
{
    return bIsHolding;
}

void UGrabber::DisplayPickupMessage()
{
    TArray<FInputActionKeyMapping> ActionMappings;
    UInputSettings::GetInputSettings()->GetActionMappingByName("Grab", ActionMappings);

    if (ActionMappings.Num() > 0)
    {
        FInputActionKeyMapping KeyMapping = ActionMappings[0];
        FString KeyName = KeyMapping.Key.GetDisplayName().ToString();
        FString Message = FString::Printf(TEXT("Press %s to pick up"), *KeyName);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Message);
    }
}

void UGrabber::InteractWithObject()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("InteractWithObject called"));
    }
    if (GetWorld() == nullptr || CollisionComponent == nullptr) return;

    TArray<AActor*> OverlappingActors;
    CollisionComponent->GetOverlappingActors(OverlappingActors, TSubclassOf<AActor>());

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor != nullptr && Actor->ActorHasTag(FName("Interactable")))
        {
            UButtonInteractionComponent* ButtonComponent = Cast<UButtonInteractionComponent>(Actor->GetComponentByClass(UButtonInteractionComponent::StaticClass()));
            if (ButtonComponent)
            {
                ButtonComponent->OnInteract();
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Button pressed"));
                break; // Assuming you only want to interact with one object at a time
            }
        }
    }

    if (OverlappingActors.Num() == 0)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, TEXT("Nothing to interact with"));
    }
}

bool UGrabber::IsHeldItemTagged(const FName& Tag) const
{
    if (HeldItem && HeldItem->ActorHasTag(Tag))
    {
        return true;
    }
    return false;
}


