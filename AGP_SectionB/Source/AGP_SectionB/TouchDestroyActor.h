#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TouchDestroyActor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class AGP_SECTIONB_API ATouchDestroyActor : public AActor
{
	GENERATED_BODY()

public:
	ATouchDestroyActor();

	// Called by DeathZone to "spawn" platforms back
	UFUNCTION(BlueprintCallable, Category = "Reset")
	void ResetPlatform();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* Trigger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Destroy")
	float DestroyDelay = 0.5f;

	bool bTriggered = false;
	FTimerHandle DestroyTimer;

	// Remember initial state
	FTransform InitialTransform;

	UFUNCTION()
	void OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// Instead of Destroy(), we disable/hide
	void DisablePlatform();
};
