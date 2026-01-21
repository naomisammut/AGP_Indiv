#include "DeathZone.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/Character.h"

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

void ADeathZone::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	// Only affect player-controlled character (common platformer behavior)
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (!Character) return;

	// Check it’s controlled by a player (not AI)
	if (!Character->GetController() || !Character->GetController()->IsPlayerController())
		return;

	RespawnActor(Character);
}

void ADeathZone::RespawnActor(AActor* ActorToRespawn)
{
	if (!ActorToRespawn) return;

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
		if (Char->GetCharacterMovement())
		{
			Char->GetCharacterMovement()->StopMovementImmediately();
		}
	}

	ActorToRespawn->SetActorLocationAndRotation(TargetLoc, TargetRot, false, nullptr, ETeleportType::TeleportPhysics);
}
