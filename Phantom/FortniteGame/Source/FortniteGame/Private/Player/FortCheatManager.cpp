#include "pch.h"
#include "FortniteGame/Public/Player/FortCheatManager.h"
#include "Core/Public/GenericPlatform/GenericPlatformMisc.h"

void UFortCheatManager::TeleportPawnTo(APawn* Pawn, AController* Controller, FVector TeleportTargetLocation, FRotator TeleportTargetRot, bool bIsATest, bool bNoCheck)
{
	AFortPawn* FortPawn = Cast<AFortPawn>(Pawn);

	if (FortPawn != NULL)
		FortPawn->TeleportTo(TeleportTargetLocation, TeleportTargetRot, bIsATest, bNoCheck);
}

void UFortCheatManager::AddScoreStat(unsigned int ScoreStat, unsigned int Amount)
{
	if (AFortPlayerController* const MyFPC = GetOuterAFortPlayerController())
	{
		MyFPC->ClientMessage(L"Cheat Command not implemented!", FName(), 0.f);
	}
}

void FortCheatManager::AddAllScores(UFortCheatManager* Context, FFrame* Stack)
{
	int32 Amount;

	Stack->StepCompiledIn(&Amount);
	Stack->IncrementCode();

	Context->AddScoreStat(1u, Amount);
	Context->AddScoreStat(9u, Amount);
	Context->AddScoreStat(0x18u, Amount);
}

void FortCheatManager::AddBuildingScore(UFortCheatManager* Context, FFrame* Stack)
{
	int32 Amount;

	Stack->StepCompiledIn(&Amount);
	Stack->IncrementCode();

	Context->AddScoreStat(9, Amount);
}

void FortCheatManager::AddCombatScore(UFortCheatManager* Context, FFrame* Stack)
{
	int32 Amount;

	Stack->StepCompiledIn(&Amount);
	Stack->IncrementCode();

	Context->AddScoreStat(1u, Amount);
}

void FortCheatManager::AddKillFeedMessage(UFortCheatManager* Context, FFrame* Stack)
{
	Stack->IncrementCode();
}

void FortCheatManager::AddUtilityScore(UFortCheatManager* Context, FFrame* Stack)
{
	int32 Amount;

	Stack->StepCompiledIn(&Amount);
	Stack->IncrementCode();

	Context->AddScoreStat(0x18u, Amount);
}

void FortCheatManager::ApplyGameplayModifier(UFortCheatManager* Context, FFrame* Stack)
{
	FString ModifierName;

	Stack->StepCompiledIn(&ModifierName);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		MyFPC->ClientMessage(L"Cheat Command not implemented!", FName(), 0.f);
	}
}

void FortCheatManager::ApplyWeaponAlteration(UFortCheatManager* Context, FFrame* Stack)
{
	FString AlterationName;

	Stack->StepCompiledIn(&AlterationName);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		MyFPC->ClientMessage(L"Cheat Command not implemented!", FName(), 0.f);
	}
}

void FortCheatManager::AthenaEmote(UFortCheatManager* Context, FFrame* Stack)
{
	FString EmoteName;

	Stack->StepCompiledIn(&EmoteName);
	Stack->IncrementCode();

	Context->PlayAthenaEmoteOnScriptedPawn(EmoteName);
}

void FortCheatManager::AthenaEndlessGame(UFortCheatManager* Context, FFrame* Stack)
{
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		MyFPC->ClientMessage(L"Cheat Command not implemented!", FName(), 0.f);
	}
}

void FortCheatManager::AthenaForceVictory(UFortCheatManager* Context, FFrame* Stack)
{
	bool bVictory;

	Stack->StepCompiledIn(&bVictory);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		if (AFortPlayerStateAthena* PlayerStateAthena = Cast<AFortPlayerStateAthena>(MyFPC->GetPlayerState()))
		{
			if (AFortGameModeAthena* GM = MyFPC->GetWorld()->GetGameModeAthena())
			{
				GM->StartEndGamePhaseTeam((int)PlayerStateAthena->TeamIndex, PlayerStateAthena, 1, MyFPC->Pawn, NULL, EDeathCause::WonMatch);
			}
		}
	}
}

void FortCheatManager::AthenaGiveScoreTo(UFortCheatManager* Context, FFrame* Stack)
{
	int32 PlayerStateIdx;
	int32 Score;

	Stack->StepCompiledIn(&PlayerStateIdx);
	Stack->StepCompiledIn(&Score);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		MyFPC->ClientMessage(L"Cheat Command not implemented!", FName(), 0.f);
	}
}

void FortCheatManager::BackpackSetSize(UFortCheatManager* Context, FFrame* Stack)
{
	int32 Size;

	Stack->StepCompiledIn(&Size);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		MyFPC->OverriddenBackpackSize = Size;
	}
}

void FortCheatManager::Badass(UFortCheatManager* Context, FFrame* Stack)
{
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		MyFPC->ClientMessage(L"Cheat Command not implemented!", FName(), 0.f);
	}
}

void FortCheatManager::BringDownWall(UFortCheatManager* Context, FFrame* Stack)
{
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		MyFPC->ClientMessage(L"Cheat Command not implemented!", FName(), 0.f);
	}
}

void FortCheatManager::BuildFree(UFortCheatManager* Context, FFrame* Stack)
{
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		MyFPC->bBuildFree = (bool)!MyFPC->bBuildFree;
	}
}

void FortCheatManager::CraftFree(UFortCheatManager* Context, FFrame* Stack)
{
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		MyFPC->bCraftFree = (bool)!MyFPC->bCraftFree;
	}
}

void FortCheatManager::ForceServerShutdown(UFortCheatManager* Context, FFrame* Stack)
{
	int32 ExitCode;

	Stack->StepCompiledIn(&ExitCode);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		if (MyFPC->Role == ENetRole::ROLE_Authority)
		{
			if (AFortGameModeAthena* GM = MyFPC->GetWorld()->GetGameModeAthena())
			{
				AGameSession* GameSession = GM->GameSession;

				if (AFortGameSessionDedicated* GameSessionDedicated = Cast<AFortGameSessionDedicated>(GameSession))
					GameSessionDedicated->ShutdownDedicatedServer(ExitCode);
			}
		}
	}
}

void FortCheatManager::GiveAllIngredients(UFortCheatManager* Context, FFrame* Stack)
{
	int32 NumIngredients;

	Stack->StepCompiledIn(&NumIngredients);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		TArray<UObject*> Ingredients = UFortKismetLibrary::JonLHack_GetAllObjectsOfClassFromPath(L"/Game/Items/Ingredients", UFortIngredientItemDefinition::StaticClass());

		for (UObject* Ingredient : Ingredients)
		{
			UFortIngredientItemDefinition* FortIngredientItemDefinition = Cast<UFortIngredientItemDefinition>(Ingredient);

			if (FortIngredientItemDefinition != NULL)
			{
				MyFPC->WorldInventory->AddItemStack(FortIngredientItemDefinition, NumIngredients);
			}
		}
	}
}

void FortCheatManager::GiveConsumable(UFortCheatManager* Context, FFrame* Stack)
{
	FString ConsumableName;
	int32 RequestedLevel;
	int32 Count;

	Stack->StepCompiledIn(&ConsumableName);
	Stack->StepCompiledIn(&RequestedLevel);
	Stack->StepCompiledIn(&Count);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		UFortItemDefinition* FortItemDefinition = Utils::StaticFindObject<UFortItemDefinition>(ConsumableName.CStr(), ANY_PACKAGE);

		if (FortItemDefinition == NULL)
		{
			MyFPC->ClientMessage(L"Couldn't find ItemDefinition from the request ConsumableName!", FName(), 0.f);
			return;
		}

		MyFPC->WorldInventory->AddItem(FortItemDefinition, Count);
	}
}

void FortCheatManager::GiveGadget(UFortCheatManager* Context, FFrame* Stack)
{
	FString GadgetName;
	int32 ItemLevel;

	Stack->StepCompiledIn(&GadgetName);
	Stack->StepCompiledIn(&ItemLevel);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		UFortGadgetItemDefinition* FortGadgetItemDefinition = Utils::StaticFindObject<UFortGadgetItemDefinition>(GadgetName.CStr(), ANY_PACKAGE);

		if (FortGadgetItemDefinition == NULL)
		{
			MyFPC->ClientMessage(L"Couldn't find FortGadgetItemDefinition from the request GadgetName!", FName(), 0.f);
			return;
		}

		MyFPC->WorldInventory->AddItem(FortGadgetItemDefinition, 1);
	}
}

void FortCheatManager::GiveGadgets(UFortCheatManager* Context, FFrame* Stack)
{
	FString GadgetName;
	int32 Quantity;
	int32 ItemLevel;

	Stack->StepCompiledIn(&GadgetName);
	Stack->StepCompiledIn(&Quantity);
	Stack->StepCompiledIn(&ItemLevel);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		UFortGadgetItemDefinition* FortGadgetItemDefinition = Utils::StaticFindObject<UFortGadgetItemDefinition>(GadgetName.CStr(), ANY_PACKAGE);

		if (FortGadgetItemDefinition == NULL)
		{
			MyFPC->ClientMessage(L"Couldn't find FortGadgetItemDefinition from the request GadgetName!", FName(), 0.f);
			return;
		}

		MyFPC->WorldInventory->AddItem(FortGadgetItemDefinition, Quantity);
	}
}

void FortCheatManager::GivePickaxe(UFortCheatManager* Context, FFrame* Stack)
{
	FString PickName;

	Stack->StepCompiledIn(&PickName);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		UAthenaPickaxeItemDefinition* PickaxeItemDefinition = Utils::StaticFindObject<UAthenaPickaxeItemDefinition>(PickName.CStr(), ANY_PACKAGE);

		if (PickaxeItemDefinition == NULL)
		{
			MyFPC->ClientMessage(L"Couldn't find PickaxeItemDefinition from the request PickName!", FName(), 0.f);
			return;
		}

		MyFPC->WorldInventory->AddItem(PickaxeItemDefinition, 1);
	}
}

void FortCheatManager::GiveBluGlo(UFortCheatManager* Context, FFrame* Stack)
{
	int32 Amount;

	Stack->StepCompiledIn(&Amount);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		UFortNeverPersistItemDefinition* BluGloItemDefinition = UFortGameData::Get()->BluGloItemDefinition.LoadSynchronous();

		if (BluGloItemDefinition != NULL)
			MyFPC->WorldInventory->AddItemStack(BluGloItemDefinition, Amount);
	}
}

void FortCheatManager::SetTeam(UFortCheatManager* Context, FFrame* Stack)
{
	int32 TeamIndex;

	Stack->StepCompiledIn(&TeamIndex);
	Stack->IncrementCode();

	AFortPlayerControllerAthena* FortPlayerControllerAthena = Cast<AFortPlayerControllerAthena>(Context->Outer);

	if (FortPlayerControllerAthena == NULL)
		return;

	FortPlayerControllerAthena->ServerSetTeam((EFortTeam)TeamIndex);
}

void FortCheatManager::GiveResources(UFortCheatManager* Context, FFrame* Stack)
{
	int32 Amount;

	Stack->StepCompiledIn(&Amount);
	Stack->IncrementCode();

	Context->GiveMetal(Amount);
	Context->GiveWood(Amount);
	Context->GiveStone(Amount);
}

void FortCheatManager::GiveSpecificItem(UFortCheatManager* Context, FFrame* Stack)
{
	FString ItemPath;
	int32 ItemAmount;
	int32 ItemLevel;

	Stack->StepCompiledIn(&ItemPath);
	Stack->StepCompiledIn(&ItemAmount);
	Stack->StepCompiledIn(&ItemLevel);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		UFortItemDefinition* FortItemDefinition = Utils::StaticFindObject<UFortItemDefinition>(ItemPath.CStr());

		if (FortItemDefinition == NULL)
		{
			MyFPC->ClientMessage(L"Couldn't find FortItemDefinition from the request ItemPath!", FName(), 0.f);
			return;
		}

		MyFPC->WorldInventory->AddItem(FortItemDefinition, ItemAmount);
	}
}

void FortCheatManager::GiveTrap(UFortCheatManager* Context, FFrame* Stack)
{
	FString TrapName;
	int32 RequestedLevel;
	int32 NumTrapsToGive;

	Stack->StepCompiledIn(&TrapName);
	Stack->StepCompiledIn(&RequestedLevel);
	Stack->StepCompiledIn(&NumTrapsToGive);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		UFortTrapItemDefinition* FortTrapItemDefinition = Utils::StaticFindObject<UFortTrapItemDefinition>(TrapName.CStr(), ANY_PACKAGE);

		if (FortTrapItemDefinition == NULL)
		{
			MyFPC->ClientMessage(L"Couldn't find FortTrapItemDefinition from the request TrapName!", FName(), 0.f);
			return;
		}

		MyFPC->WorldInventory->AddItem(FortTrapItemDefinition, NumTrapsToGive);
	}
}

void FortCheatManager::GiveWeapon(UFortCheatManager* Context, FFrame* Stack)
{
	FString WeaponName;
	int32 RequestedLevel;
	int32 Count;

	Stack->StepCompiledIn(&WeaponName);
	Stack->StepCompiledIn(&RequestedLevel);
	Stack->StepCompiledIn(&Count);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		UFortWeaponItemDefinition* FortWeaponItemDefinition = Utils::StaticFindObject<UFortWeaponItemDefinition>(WeaponName.CStr(), ANY_PACKAGE);

		if (FortWeaponItemDefinition == NULL)
		{
			MyFPC->ClientMessage(L"Couldn't find weapon from the request WeaponName!", FName(), 0.f);
			return;
		}

		MyFPC->WorldInventory->AddItem(FortWeaponItemDefinition, Count);
	}
}

void FortCheatManager::GoFast(UFortCheatManager* Context, FFrame* Stack)
{
	float NewSpeed;

	Stack->StepCompiledIn(&NewSpeed);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		MyFPC->ClientMessage(L"Cheat Command not implemented!", FName(), 0.f);
	}
}

void FortCheatManager::GiveWood(UFortCheatManager* Context, FFrame* Stack)
{
	int32 Amount;

	Stack->StepCompiledIn(&Amount);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		UFortResourceItemDefinition* WoodItemDefinition = UFortGameData::Get()->WoodItemDefinition.LoadSynchronous();

		if (WoodItemDefinition != NULL)
		{
			MyFPC->WorldInventory->AddItemStack(WoodItemDefinition, Amount);
		}
	}
}

void FortCheatManager::GiveMetal(UFortCheatManager* Context, FFrame* Stack)
{
	int32 Amount;

	Stack->StepCompiledIn(&Amount);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		UFortResourceItemDefinition* MetalItemDefinition = UFortGameData::Get()->MetalItemDefinition.LoadSynchronous();

		if (MetalItemDefinition != NULL)
		{
			MyFPC->WorldInventory->AddItemStack(MetalItemDefinition, Amount);
		}
	}
}

void FortCheatManager::GiveStone(UFortCheatManager* Context, FFrame* Stack)
{
	int32 Amount;

	Stack->StepCompiledIn(&Amount);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		UFortResourceItemDefinition* StoneItemDefinition = UFortGameData::Get()->StoneItemDefinition.LoadSynchronous();

		if (StoneItemDefinition != NULL)
		{
			MyFPC->WorldInventory->AddItemStack(StoneItemDefinition, Amount);
		}
	}
}

void FortCheatManager::MassSuicide(UFortCheatManager* Context, FFrame* Stack)
{
	Stack->IncrementCode();

	for (AFortPlayerControllerAthena* FortPlayerController : GWorld->GetGameModeAthena()->AlivePlayers)
	{
		FortPlayerController->Suicide();
	}
}

void FortCheatManager::SetHealthPercent(UFortCheatManager* Context, FFrame* Stack)
{
	float Percent;

	Stack->StepCompiledIn(&Percent);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		if (AFortPlayerPawn* PlayerPawn = MyFPC->GetPlayerPawn())
		{
			if (Percent > PlayerPawn->GetMaxHealth())
				return;

			PlayerPawn->SetHealth(Percent);
		}
	}
}

void FortCheatManager::SetShieldPercent(UFortCheatManager* Context, FFrame* Stack)
{
	float Percent;

	Stack->StepCompiledIn(&Percent);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		if (AFortPlayerPawn* PlayerPawn = MyFPC->GetPlayerPawn())
		{
			if (Percent > PlayerPawn->GetMaxShield())
				return;

			PlayerPawn->SetShield(Percent);
		}
	}
}

void FortCheatManager::TeleportToLocation(UFortCheatManager* Context, FFrame* Stack)
{
	float X;
	float Y;
	float Z;

	Stack->StepCompiledIn(&X);
	Stack->StepCompiledIn(&Y);
	Stack->StepCompiledIn(&Z);
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		MyFPC->ServerTeleportToReticle(FVector(X, Y, Z));
	}
}

void FortCheatManager::ToggleInfiniteAmmo(UFortCheatManager* Context, FFrame* Stack)
{
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		MyFPC->bInfiniteAmmo = (bool)!MyFPC->bInfiniteAmmo;

		FString Message;
		FString::PrintfImpl(&Message, MyFPC->bInfiniteAmmo ? L"Infinite ammo is true" : L"Infinite ammo is false");

		MyFPC->ClientMessage(Message, FName(), 0.f);
	}
}

void FortCheatManager::ToggleUnlimitedHealth(UFortCheatManager* Context, FFrame* Stack)
{
	Stack->IncrementCode();

	if (AFortPlayerController* const MyFPC = Context->GetOuterAFortPlayerController())
	{
		if (AFortPlayerPawn* PlayerPawn = MyFPC->GetPlayerPawn())
		{
			PlayerPawn->bIsInvulnerable = !PlayerPawn->bIsInvulnerable;

			FString Message;
			FString::PrintfImpl(&Message, PlayerPawn->bIsInvulnerable ? L"Unlimited health is true" : L"Unlimited health is false");

			MyFPC->ClientMessage(Message, FName(), 0.f);
		}
	}
}

void FortCheatManager::Setup()
{
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.AddAllScores"), AddAllScores);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.AddBuildingScore"), AddBuildingScore);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.AddCombatScore"), AddCombatScore);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.AddKillFeedMessage"), AddKillFeedMessage);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.AddUtilityScore"), AddUtilityScore);

	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.ApplyGameplayModifier"), ApplyGameplayModifier);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.ApplyWeaponAlteration"), ApplyWeaponAlteration);

	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.AthenaEmote"), AthenaEmote);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.AthenaEndlessGame"), AthenaEndlessGame);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.AthenaForceVictory"), AthenaForceVictory);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.AthenaGiveScoreTo"), AthenaGiveScoreTo);

	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.BackpackSetSize"), BackpackSetSize);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.Badass"), Badass);

	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.BuildFree"), BuildFree);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.CraftFree"), CraftFree);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.ForceServerShutdown"), ForceServerShutdown);

	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.GiveAllIngredients"), GiveAllIngredients);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.GiveConsumable"), GiveConsumable);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.GiveGadget"), GiveGadget);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.GiveGadgets"), GiveGadgets);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.GivePickaxe"), GivePickaxe);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.GiveResources"), GiveResources);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.GiveSpecificItem"), GiveSpecificItem);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.GiveTrap"), GiveTrap);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.GiveWeapon"), GiveWeapon);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.GiveWood"), GiveWood);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.GoFast"), GoFast);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.GiveMetal"), GiveMetal);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.GiveStone"), GiveStone);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.GiveBluGlo"), GiveBluGlo);

	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.MassSuicide"), MassSuicide);

	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.SetHealthPercent"), SetHealthPercent);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.SetShieldPercent"), SetShieldPercent);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.SetTeam"), SetTeam);

	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.TeleportToLocation"), TeleportToLocation);

	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.ToggleInfiniteAmmo"), ToggleInfiniteAmmo);
	Utils::ExecHook(TEXT("/Script/FortniteGame.FortCheatManager.ToggleUnlimitedHealth"), ToggleUnlimitedHealth);

	Utils::Virtual(UFortCheatManager::GetDefaultObj()->VTable, 0x260 / 8, UCheatManager::GetDefaultObj()->VTable[0x260 / 8]); // Fly
	Utils::Virtual(UFortCheatManager::GetDefaultObj()->VTable, 0x278 / 8, UCheatManager::GetDefaultObj()->VTable[0x278 / 8]); // God
	Utils::Virtual(UFortCheatManager::GetDefaultObj()->VTable, 0x270 / 8, UCheatManager::GetDefaultObj()->VTable[0x270 / 8]); // Ghost
	Utils::Virtual(UFortCheatManager::GetDefaultObj()->VTable, 0x268 / 8, UCheatManager::GetDefaultObj()->VTable[0x268 / 8]); // Walk
	Utils::Virtual(UFortCheatManager::GetDefaultObj()->VTable, 0x280 / 8, UCheatManager::GetDefaultObj()->VTable[0x280 / 8]); // Slomo
}