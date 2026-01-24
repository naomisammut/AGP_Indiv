#include "CheckpointComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"

UCheckpointComponent::UCheckpointComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCheckpointComponent::SetCheckpoint(const FTransform& NewCheckpoint)
{
	CheckpointTransform = NewCheckpoint;
	bHasCheckpoint = true;
}

void UCheckpointComponent::ClearCheckpoint()
{
	bHasCheckpoint = false;
	CheckpointTransform = FTransform::Identity;
}

void UCheckpointComponent::Respawn(AController* ControllerOverride)
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar) return;

	// Stop movement so teleport is clean
	if (UCharacterMovementComponent* Move = OwnerChar->GetCharacterMovement())
	{
		Move->StopMovementImmediately();
	}

	// Use checkpoint if we have one
	if (bHasCheckpoint)
	{
		OwnerChar->TeleportTo(
			CheckpointTransform.GetLocation(),
			CheckpointTransform.Rotator(),
			false, true
		);
		return;
	}

	// Otherwise -> PlayerStart
	AController* Ctrl = ControllerOverride ? ControllerOverride : OwnerChar->GetController();
	if (!Ctrl) return;

	AGameModeBase* GM = OwnerChar->GetWorld()->GetAuthGameMode();
	if (!GM) return;

	AActor* Start = GM->FindPlayerStart(Ctrl);
	if (!Start) return;

	OwnerChar->TeleportTo(Start->GetActorLocation(), Start->GetActorRotation(), false, true);
}
