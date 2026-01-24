#include "AGPGameplayDebuggerCategory.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EngineUtils.h"

#include "CheckpointComponent.h"
#include "SpawnButton.h"

FAGPGameplayDebuggerCategory::FAGPGameplayDebuggerCategory()
{
}

TSharedRef<FGameplayDebuggerCategory> FAGPGameplayDebuggerCategory::MakeInstance()
{
	return MakeShareable(new FAGPGameplayDebuggerCategory());
}

void FAGPGameplayDebuggerCategory::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
	ACharacter* Char = Cast<ACharacter>(DebugActor);
	if (!Char && OwnerPC) Char = Cast<ACharacter>(OwnerPC->GetPawn());
	if (!Char) return;

	// Speed
	Speed = Char->GetVelocity().Size();

	// Falling
	if (UCharacterMovementComponent* Move = Char->GetCharacterMovement())
		bIsFalling = Move->IsFalling();

	// Checkpoint
	if (UCheckpointComponent* CP = Char->FindComponentByClass<UCheckpointComponent>())
	{
		bHasCheckpoint = CP->bHasCheckpoint;
		CheckpointLoc = bHasCheckpoint ? CP->CheckpointTransform.GetLocation() : FVector::ZeroVector;
	}

	// Button state (checks any SpawnButton in the level)
	bButtonPressed = false;
	bActorsActive = false;

	if (UWorld* World = Char->GetWorld())
	{
		for (TActorIterator<ASpawnButton> It(World); It; ++It)
		{
			if (ASpawnButton* B = *It)
			{
				bButtonPressed |= B->IsButtonPressed();
				bActorsActive |= B->AreActorsActive();
			}
		}
	}
}

void FAGPGameplayDebuggerCategory::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	CanvasContext.Printf(TEXT("{yellow}AGP Debug"));
	CanvasContext.Printf(TEXT("Speed: {green}%.1f"), Speed);
	CanvasContext.Printf(TEXT("IsFalling: {green}%s"), bIsFalling ? TEXT("True") : TEXT("False"));
	CanvasContext.Printf(TEXT("HasCheckpoint: {green}%s"), bHasCheckpoint ? TEXT("True") : TEXT("False"));
	CanvasContext.Printf(TEXT("CheckpointLoc: {green}%s"), *CheckpointLoc.ToString());
	CanvasContext.Printf(TEXT("ButtonPressed: {green}%s"), bButtonPressed ? TEXT("True") : TEXT("False"));
	CanvasContext.Printf(TEXT("Bridge/Lasers Active: {green}%s"), bActorsActive ? TEXT("True") : TEXT("False"));
}
