#include "SpawnButton.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

ASpawnButton::ASpawnButton()
{
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	RootComponent = Trigger;

	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Trigger->SetGenerateOverlapEvents(true);

	Trigger->CanCharacterStepUpOn = ECB_No;
	Trigger->SetBoxExtent(FVector(50.f, 50.f, 50.f));
}

void ASpawnButton::BeginPlay()
{
	Super::BeginPlay();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASpawnButton::OnTriggerBegin);

	// Hide all assigned actors at start
	for (AActor* A : ActorsToReveal)
	{
		if (!A) continue;

		A->SetActorHiddenInGame(true);
		A->SetActorEnableCollision(false);
		A->SetActorTickEnabled(false);
	}
}

void ASpawnButton::OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (bUsed) return;
	if (!OtherActor) return;

	// Only react to player character
	if (!OtherActor->IsA(ACharacter::StaticClass()))
		return;

	bUsed = true;
	RevealActors();
}

void ASpawnButton::RevealActors()
{
	for (AActor* A : ActorsToReveal)
	{
		if (!A) continue;

		A->SetActorHiddenInGame(false);
		A->SetActorEnableCollision(true);
		A->SetActorTickEnabled(true);
	}
}
