#include "SpawnButton.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

ASpawnButton::ASpawnButton()
{
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	RootComponent = Trigger;

	// Good default trigger settings
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Trigger->SetGenerateOverlapEvents(true);

	// Make it feel like a trigger, not a step/platform
	Trigger->CanCharacterStepUpOn = ECB_No;

	// Reasonable default size (you can change in editor)
	Trigger->SetBoxExtent(FVector(50.f, 50.f, 50.f));
}

void ASpawnButton::BeginPlay()
{
	Super::BeginPlay();

	// Bind overlap event
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASpawnButton::OnTriggerBegin);

	// Hide the bridge at start (if assigned)
	if (BridgeActor)
	{
		BridgeActor->SetActorHiddenInGame(true);
		BridgeActor->SetActorEnableCollision(false);
		BridgeActor->SetActorTickEnabled(false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnButton: BridgeActor NOT assigned on %s"), *GetName());
	}
}

void ASpawnButton::OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (bUsed) return;
	if (!OtherActor) return;

	// Only react to the player character (simple + reliable)
	if (!OtherActor->IsA(ACharacter::StaticClass()))
		return;

	UE_LOG(LogTemp, Warning, TEXT("SpawnButton pressed by %s"), *OtherActor->GetName());

	bUsed = true;
	RevealBridge();
}

void ASpawnButton::RevealBridge()
{
	if (!BridgeActor)
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnButton: RevealBridge called but BridgeActor is NULL on %s"), *GetName());
		return;
	}

	BridgeActor->SetActorHiddenInGame(false);
	BridgeActor->SetActorEnableCollision(true);
	BridgeActor->SetActorTickEnabled(true);

	UE_LOG(LogTemp, Warning, TEXT("SpawnButton: Bridge revealed: %s"), *BridgeActor->GetName());
}
