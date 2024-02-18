// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/StaticMeshComponent.h"
#include "UIObjectPreviewActor.generated.h"

UCLASS()
class AUIObjectPreviewActor : public AActor
{
    GENERATED_BODY()

public:
    AUIObjectPreviewActor();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
        USceneCaptureComponent2D* SceneCaptureComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
        UStaticMeshComponent* MeshComponent;

    void SetPreviewMesh(UStaticMesh* NewMesh);
};