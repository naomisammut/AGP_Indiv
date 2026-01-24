#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VLogPlayerComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AGP_SECTIONB_API UVLogPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UVLogPlayerComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
