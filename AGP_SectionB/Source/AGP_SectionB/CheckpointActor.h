#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckpointActor.generated.h"

class UBoxComponent;

UCLASS()
class AGP_SECTIONB_API ACheckpointActor : public AActor
{
	GENERATED_BODY()

public:
	ACheckpointActor();

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* Trigger;

	// Move this in editor to control exact respawn spot
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SpawnPoint;

	UFUNCTION()
	void OnOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};
