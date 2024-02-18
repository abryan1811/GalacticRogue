// Fill out your copyright notice in the Description page of Project Settings.


#include "UIObjectPreviewActor.h"

// Sets default values
AUIObjectPreviewActor::AUIObjectPreviewActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Setup the components
    SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

    // Attach components and set the render target
    RootComponent = MeshComponent;
    SceneCaptureComponent->SetupAttachment(RootComponent);

}

void AUIObjectPreviewActor::SetPreviewMesh(UStaticMesh* NewMesh)
{
    if (MeshComponent)
    {
        MeshComponent->SetStaticMesh(NewMesh);
        // Optionally set the material of the mesh component if needed
    }
}

