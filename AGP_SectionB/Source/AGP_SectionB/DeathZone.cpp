#include "DeathZone.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "CheckpointComponent.h"
#include "TouchDestroyActor.h"

ADeathZone::ADeathZone()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("DeathZoneBox"));
	RootComponent = Box;

	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionObjectType(ECC_WorldDynamic);

	// Overlap pawns/characters
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	Box->SetGenerateOverlapEvents(true);

	Box->OnComponentBeginOverlap.AddDynamic(this, &ADeathZone::OnBoxBeginOverlap);
}

// Helper: reset all disappearing platforms in the level
static void ResetAllTouchPlatforms(UWorld* World)
{
	if (!World) return;

	TArray<AActor*> Platforms;
	UGameplayStatics::GetAllActorsOfClass(World, ATouchDestroyActor::StaticClass(), Platforms);

	for (AActor* A : Platforms)
	{
		if (ATouchDestroyActor* P = Cast<ATouchDestroyActor>(A))
		{
			P->ResetPlatform();
		}
	}
}

void ADeathZone::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	// Only affect player-controlled character
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (!Character) return;

	// Must be player controlled
	if (!Character->GetController() || !Character->GetController()->IsPlayerController())
		return;

	RespawnActor(Character);
}

void ADeathZone::RespawnActor(AActor* ActorToRespawn)
{
	if (!ActorToRespawn) return;

	// ✅ Reset platforms when player dies/falls
	ResetAllTouchPlatforms(GetWorld());

	// ✅ Respawn to checkpoint if available, else PlayerStart
	if (UCheckpointComponent* CP = ActorToRespawn->FindComponentByClass<UCheckpointComponent>())
	{
		ACharacter* Char = Cast<ACharacter>(ActorToRespawn);
		CP->Respawn(Char ? Char->GetController() : nullptr);
		return;
	}

	// ---- Fallback (old system) ----
	FVector TargetLoc = SpawnLocation;
	FRotator TargetRot = FRotator::ZeroRotator;

	if (bUsePlayerStart)
	{
		AActor* PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
		if (PlayerStart)
		{
			TargetLoc = PlayerStart->GetActorLocation();
			TargetRot = PlayerStart->GetActorRotation();
		}
	}

	// Reset movement velocity if it’s a character
	if (ACharacter* Char = Cast<ACharacter>(ActorToRespawn))
	{
		if (UCharacterMovementComponent* Move = Char->GetCharacterMovement())
		{
			Move->StopMovementImmediately();
		}
	}

	ActorToRespawn->SetActorLocationAndRotation(
		TargetLoc,
		TargetRot,
		false,
		nullptr,
		ETeleportType::TeleportPhysics
	);
}
