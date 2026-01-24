#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnButton.generated.h"

class UBoxComponent;

UCLASS()
class AGP_SECTIONB_API ASpawnButton : public AActor
{
	GENERATED_BODY()

public:
	ASpawnButton();

protected:
	virtual void BeginPlay() override;

	// Trigger volume you walk into
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Button")
	UBoxComponent* Trigger;

	// Actors to hide at start and reveal when pressed (set in Details)
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Button")
	TArray<AActor*> ActorsToReveal;

	// Prevent pressing more than once
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Button")
	bool bUsed = false;

	// Overlap callback
	UFUNCTION()
	void OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// Reveal all actors
	void RevealActors();
};
