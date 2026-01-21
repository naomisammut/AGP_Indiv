#include "MovingPillar.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"

AMovingPillar::AMovingPillar()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	KillTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("KillTrigger"));
	KillTrigger->SetupAttachment(RootComponent);

	KillTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	KillTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	KillTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	KillTrigger->SetGenerateOverlapEvents(true);

	KillTrigger->OnComponentBeginOverlap.AddDynamic(
		this, &AMovingPillar::OnPlayerHit
	);
}

void AMovingPillar::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
}

void AMovingPillar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Offset = GetActorUpVector() * MoveSpeed * Direction * DeltaTime;
	AddActorWorldOffset(Offset);

	if (FVector::Dist(StartLocation, GetActorLocation()) >= MoveDistance)
	{
		Direction *= -1.f;
	}
}

void AMovingPillar::OnPlayerHit(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	ACharacter* Player = Cast<ACharacter>(OtherActor);
	if (!Player) return;

	// Send player back to start
	Player->SetActorLocation(Player->GetWorld()->GetAuthGameMode()->FindPlayerStart(Player->GetController())->GetActorLocation());
}
