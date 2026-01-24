#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CheckpointComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AGP_SECTIONB_API UCheckpointComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCheckpointComponent();

	// Called by checkpoint actor when player touches it
	UFUNCTION(BlueprintCallable, Category = "Checkpoint")
	void SetCheckpoint(const FTransform& NewCheckpoint);

	// Optional: reset checkpoint (not required)
	UFUNCTION(BlueprintCallable, Category = "Checkpoint")
	void ClearCheckpoint();

	// Called by your kill actors (MovingPillar/DeathZone)
	// Respawns to checkpoint if set, otherwise PlayerStart.
	UFUNCTION(BlueprintCallable, Category = "Checkpoint")
	void Respawn(AController* ControllerOverride = nullptr);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Checkpoint")
	bool bHasCheckpoint = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Checkpoint")
	FTransform CheckpointTransform;
};
