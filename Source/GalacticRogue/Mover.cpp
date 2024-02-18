// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"
#include "Math/UnrealMathUtility.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetActorLocation();

	OriginalRotation = GetOwner()->GetActorRotation();

}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    FVector TargetLocation = OriginalLocation;
    FRotator TargetRotation = OriginalRotation;

    if (ShouldMove)
    {
        if (!bIsCurrentlyMoving)
        {
            bIsCurrentlyMoving = true;
            if (SlidingDoorSoundCue && GetOwner()->ActorHasTag(FName("spaceship door")))
            {
                UGameplayStatics::PlaySoundAtLocation(this, SlidingDoorSoundCue, GetOwner()->GetActorLocation());
            }
        }
        TargetLocation = OriginalLocation + MoveOffset;
        TargetRotation = OriginalRotation + RotationOffset;
    }
    else
    {
        if (bIsCurrentlyMoving)
        {
            bIsCurrentlyMoving = false;
        }
    }

	FVector CurrentLocation = GetOwner()->GetActorLocation();

	float MoveSpeed = MoveOffset.Length() / MoveTime;

	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed);
	GetOwner()->SetActorLocation(NewLocation);

	FRotator CurrentRotation = GetOwner()->GetActorRotation();

	float RotationSpeed = RotationOffset.Yaw / RotationTime;

	FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
	GetOwner()->SetActorRotation(NewRotation);
}

void UMover::SetShouldMove(bool NewShouldMove)
{
	ShouldMove = NewShouldMove;
}