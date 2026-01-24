// Copyright Epic Games, Inc. All Rights Reserved.

#include "AGP_SectionB.h"
#include "Modules/ModuleManager.h"

#if WITH_GAMEPLAY_DEBUGGER
#include "GameplayDebugger.h"
#include "AGPGameplayDebuggerCategory.h"
#endif

class FAGP_SectionBModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override
	{
#if WITH_GAMEPLAY_DEBUGGER
		IGameplayDebugger::Get().RegisterCategory(
			"AGP",
			IGameplayDebugger::FOnGetCategory::CreateStatic(&FAGPGameplayDebuggerCategory::MakeInstance),
			EGameplayDebuggerCategoryState::EnabledInGameAndSimulate
		);

		IGameplayDebugger::Get().NotifyCategoriesChanged();
#endif
	}

	virtual void ShutdownModule() override
	{
#if WITH_GAMEPLAY_DEBUGGER
		if (IGameplayDebugger::IsAvailable())
		{
			IGameplayDebugger::Get().UnregisterCategory("AGP");
		}
#endif
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FAGP_SectionBModule, AGP_SectionB, "AGP_SectionB");

DEFINE_LOG_CATEGORY(LogAGP_SectionB);
