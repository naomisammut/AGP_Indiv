#pragma once

#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"

class FAGPGameplayDebuggerCategory : public FGameplayDebuggerCategory
{
public:
	FAGPGameplayDebuggerCategory();

	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;

private:
	// 5+ variables to show
	bool bHasCheckpoint = false;
	FVector CheckpointLoc = FVector::ZeroVector;

	float Speed = 0.f;
	bool bIsFalling = false;

	bool bButtonPressed = false;
	bool bActorsActive = false;
};
