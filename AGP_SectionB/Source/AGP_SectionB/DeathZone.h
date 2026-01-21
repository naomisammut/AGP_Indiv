#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeathZone.generated.h"

class UBoxComponent;

UCLASS()
class AGP_SECTIONB_API ADeathZone : public AActor
{
	GENERATED_BODY()

public:
	ADeathZone();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DeathZone")
	UBoxComponent* Box;

	// If true: send player to PlayerStart (recommended)
	// If false: send player to SpawnLocation (you set it in editor)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeathZone")
	bool bUsePlayerStart = true;

	// Used only if bUsePlayerStart is false
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeathZone", meta = (EditCondition = "!bUsePlayerStart"))
	FVector SpawnLocation = FVector::ZeroVector;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	void RespawnActor(AActor* ActorToRespawn);
};
