// AmbientSoundComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "AmbientSoundComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GALACTICROGUE_API UAmbientSoundComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAmbientSoundComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void HandleEntranceOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void HandleExitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Trigger")
		bool bShouldTriggerPlaySound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		USoundCue* AmbientSoundCue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")
		UBoxComponent* SoundOnBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")
		UBoxComponent* SoundOffBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Trigger")
		FName StopSoundTag;

private:
	UAudioComponent* AudioComponent; 
};
