#include "TouchDestroyActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"

ATouchDestroyActor::ATouchDestroyActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	// Solid so player can stand on it (optional, but usually desired)
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);

	// Trigger is overlap-only
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Trigger->SetGenerateOverlapEvents(true);

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ATouchDestroyActor::OnTriggerBegin);
}

void ATouchDestroyActor::BeginPlay()
{
	Super::BeginPlay();
}

void ATouchDestroyActor::OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (bTriggered) return;

	// Only react to player character (safe default)
	ACharacter* Char = Cast<ACharacter>(OtherActor);
	if (!Char) return;
	if (!Char->GetController() || !Char->GetController()->IsPlayerController()) return;

	bTriggered = true;

	// Destroy after delay
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &ATouchDestroyActor::DestroySelf, DestroyDelay, false);
}

void ATouchDestroyActor::DestroySelf()
{
	Destroy();
}
