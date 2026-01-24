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

	// Useful for Gameplay Debugger
	bool IsButtonPressed() const { return bButtonPressed; }
	bool AreActorsActive() const { return bActorsActive; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Button")
	UBoxComponent* Trigger;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Button")
	TArray<AActor*> ActorsToReveal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Button")
	bool bUsed = false;

	// ✅ state variables (for debugger)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Button|State")
	bool bButtonPressed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Button|State")
	bool bActorsActive = false;

	UFUNCTION()
	void OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	void RevealActors();
};
