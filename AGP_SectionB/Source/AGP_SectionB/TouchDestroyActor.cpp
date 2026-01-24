#include "TouchDestroyActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"

ATouchDestroyActor::ATouchDestroyActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	// Solid so player can stand on it
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);

	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Trigger->SetGenerateOverlapEvents(true);

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ATouchDestroyActor::OnTriggerBegin);
}

void ATouchDestroyActor::BeginPlay()
{
	Super::BeginPlay();

	InitialTransform = GetActorTransform();
}

void ATouchDestroyActor::OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (bTriggered) return;

	ACharacter* Char = Cast<ACharacter>(OtherActor);
	if (!Char) return;

	if (!Char->GetController() || !Char->GetController()->IsPlayerController())
		return;

	bTriggered = true;

	// After delay, "remove" platform (hide + disable collision)
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &ATouchDestroyActor::DisablePlatform, DestroyDelay, false);
}

void ATouchDestroyActor::DisablePlatform()
{
	// Hide and turn off collision
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void ATouchDestroyActor::ResetPlatform()
{
	// Cancel pending timer if any
	GetWorldTimerManager().ClearTimer(DestroyTimer);

	// Restore state
	bTriggered = false;

	// Optional: snap back to original transform (useful if anything moved it)
	SetActorTransform(InitialTransform);

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(false); // you don't tick anyway
}
