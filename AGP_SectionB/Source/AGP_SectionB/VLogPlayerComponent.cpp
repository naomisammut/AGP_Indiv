#include "VLogPlayerComponent.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VisualLogger/VisualLogger.h"

#include "CheckpointComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogAGPVLog, Log, All);

UVLogPlayerComponent::UVLogPlayerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UVLogPlayerComponent::BeginPlay()
{
	Super::BeginPlay();

#if ENABLE_VISUAL_LOG
	AActor* Owner = GetOwner();
	if (!Owner) return;

	// This should be BP_ThirdPersonCharacter_C_0 if attached correctly
	UE_VLOG(Owner, LogAGPVLog, Warning, TEXT("VLOG COMPONENT OWNER: %s  CLASS: %s"),
		*Owner->GetName(), *Owner->GetClass()->GetName());

	// Also drop a visible shape immediately
	UE_VLOG_LOCATION(Owner, LogAGPVLog, Warning, Owner->GetActorLocation(), 40.f, FColor::Red, TEXT("BeginPlay Owner Loc"));
#endif
}


void UVLogPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

#if ENABLE_VISUAL_LOG
	AActor* Owner = GetOwner();
	if (!Owner) return;

	APawn* Pawn = Cast<APawn>(Owner);
	if (!Pawn) return;

	const FVector Loc = Pawn->GetActorLocation();
	const FVector Vel = Pawn->GetVelocity();
	const float Speed = Vel.Size();

	bool bIsFalling = false;
	if (ACharacter* Char = Cast<ACharacter>(Pawn))
	{
		if (UCharacterMovementComponent* Move = Char->GetCharacterMovement())
		{
			bIsFalling = Move->IsFalling();
		}
	}

	// (1) Player location (shape)
	UE_VLOG_LOCATION(Pawn, LogAGPVLog, Warning, Loc, 25.f, FColor::Green, TEXT("PlayerLoc"));

	// (2) Velocity direction (shape)
	UE_VLOG_SEGMENT(Pawn, LogAGPVLog, Warning, Loc, Loc + Vel * 0.05f, FColor::Cyan, TEXT("Velocity"));

	// (3) Speed (value)
	UE_VLOG(Pawn, LogAGPVLog, Warning, TEXT("Speed: %.1f"), Speed);

	// (4) IsFalling (value)
	UE_VLOG(Pawn, LogAGPVLog, Warning, TEXT("IsFalling: %s"), bIsFalling ? TEXT("True") : TEXT("False"));

	// (5) Checkpoint status + checkpoint location (value + shape)
	if (UCheckpointComponent* CP = Pawn->FindComponentByClass<UCheckpointComponent>())
	{
		UE_VLOG(Pawn, LogAGPVLog, Warning, TEXT("HasCheckpoint: %s"), CP->bHasCheckpoint ? TEXT("True") : TEXT("False"));

		if (CP->bHasCheckpoint)
		{
			const FVector CPLoc = CP->CheckpointTransform.GetLocation();

			UE_VLOG_LOCATION(Pawn, LogAGPVLog, Warning, CPLoc, 35.f, FColor::Yellow, TEXT("Checkpoint"));
			UE_VLOG_SEGMENT(Pawn, LogAGPVLog, Warning, Loc, CPLoc, FColor::Yellow, TEXT("ToCheckpoint"));
		}
	}
#endif
}
