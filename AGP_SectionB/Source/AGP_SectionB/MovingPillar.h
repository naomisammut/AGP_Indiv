#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPillar.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class AGP_SECTIONB_API AMovingPillar : public AActor
{
	GENERATED_BODY()

public:
	AMovingPillar();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Visual mesh
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	// Trigger to detect player hit
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* KillTrigger;

	// Movement
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveDistance = 200.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 400.f;

	FVector StartLocation;
	float Direction = 1.f;

	UFUNCTION()
	void OnPlayerHit(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};
