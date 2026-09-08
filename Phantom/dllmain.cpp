#include "pch.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Engine/Restricted/NotForLicensees/Plugins/OnlineGameplayFramework/Source/McpProfileSys/Public/McpProfileGroup.h"
#include "Engine/NetDriver.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"

#include "GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemComponent.h"
#include "GameplayAbilities/Source/GameplayAbilities/Public/Abilities/GameplayAbility.h"

#include "FortniteGame/Public/FortGameState.h"
#include "FortniteGame/Public/Athena/FortGameModeAthena.h"
#include "FortniteGame/Public/Online/FortGameSession.h"
#include "FortniteGame/Public/Player/FortPlayerControllerZone.h"
#include "FortniteGame/Public/Player/FortPlayerController.h"
#include "FortniteGame/Public/Quests/FortQuestManager.h"
#include "FortniteGame/Public/Athena/FortPoiVolume.h"
#include "FortniteGame/Public/FortGameModeZone.h"
#include "FortniteGame/Public/FortKismetLibrary.h"
#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator_Barrier.h"
#include "FortniteGame/Public/Athena/Building/AthenaBarrierFlag.h"
#include "FortniteGame/Public/Athena/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/Building/BuildingActor.h"
#include "FortniteGame/Public/Building/BuildingSMActor.h"
#include "FortniteGame/Public/Building/BuildingContainer.h"
#include "FortniteGame/Public/Athena/FortAthenaSupplyDrop.h"

#include "FortniteGame/Public/Items/FortInventoryOwnerInterface.h"
#include "FortniteGame/Public/Items/FortPickup.h"
#include "FortniteGame/Public/Items/FortWorldItem.h"
#include "FortniteGame/Public/Items/FortLootPackage.h"
#include "FortniteGame/Public/Pawns/FortPlayerPawn.h"

#include "FortniteAI/Public/FortAIDirector.h"

DWORD WINAPI LaunchWindowsStartup(LPVOID)
{
    AllocConsole();

    FILE* F = NULL;

    freopen_s(&F, "CONIN$", "r", stdin);
    freopen_s(&F, "CONOUT$", "w", stdout);
    freopen_s(&F, "CONOUT$", "w", stderr);

    SetConsoleTitleA("Phantom | Loading");

    MH_Initialize();

    World::Setup();
    FortGameState::Setup();
    FortGameModeAthena::Setup();
    McpProfileGroup::Setup();
    FortGameSession::Setup();
    NetDriver::Setup();
    FortPlayerControllerZone::Setup();
    FortPlayerController::Setup();
    AbilitySystemComponent::Setup();
    FortQuestManager::Setup();
    FortPoiVolume::Setup();
    BuildingContainer::Setup();
    FortGameModeZone::Setup();
    FortAIDirector::Setup();
    FortKismetLibrary::Setup();
    FortAthenaMutator_Barrier::Setup();
    AthenaBarrierFlag::Setup();
    FortPlayerControllerAthena::Setup();
    BuildingActor::Setup();
    BuildingSMActor::Setup();
    FortInventoryOwnerInterface::Setup();
    Object::Setup();
    GameplayAbility::Setup();
    FortPickup::Setup();
    FortPlayerPawn::Setup();
    FortWorldItem::Setup();
    FortAthenaSupplyDrop::Setup();

    Utils::Patch<uint8_t>(InSDKUtils::GetImageBase() + 0xC96B25 + 1, 0x85); // GamePhaseStep

    *GIsClient = false;
    *GIsServer = true;

    GWorld->ServerTravel(L"Athena_Terrain", false, false);
    GWorld->OwningGameInstance->RemoveLocalPlayer();

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        CreateThread(NULL, 0, LaunchWindowsStartup, NULL, 0, NULL);

    return TRUE;
}