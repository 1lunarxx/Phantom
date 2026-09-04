#include "pch.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Engine/Restricted/NotForLicensees/Plugins/OnlineGameplayFramework/Source/McpProfileSys/Public/McpProfileGroup.h"
#include "Engine/NetDriver.h"

#include "GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemComponent.h"

#include "FortniteGame/Public/FortGameState.h"
#include "FortniteGame/Public/Athena/FortGameModeAthena.h"
#include "FortniteGame/Public/Online/FortGameSession.h"
#include "FortniteGame/Public/Player/FortPlayerControllerZone.h"
#include "FortniteGame/Public/Player/FortPlayerController.h"
#include "FortniteGame/Public/Quests/FortQuestManager.h"
#include "FortniteGame/Public/Athena/FortPoiVolume.h"
#include "FortniteGame/Public/Building/BuildingContainer.h"
#include "FortniteGame/Public/FortGameModeZone.h"
#include "FortniteGame/Public/FortKismetLibrary.h"
#include "FortniteGame/Public/Missions/FortMission_RiftSpawners.h"
#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator_Barrier.h"
#include "FortniteGame/Public/Athena/Building/AthenaBarrierFlag.h"
#include "FortniteGame/Public/Athena/FortPlayerControllerAthena.h"

#include "FortniteAI/Public/FortAIDirector.h"

bool ReturnTrue() { return true; }

DWORD WINAPI LaunchWindowsStartup(LPVOID)
{
    AllocConsole();

    FILE* F = NULL;

    freopen_s(&F, "CONIN$", "r", stdin);
    freopen_s(&F, "CONOUT$", "w", stdout);
    freopen_s(&F, "CONOUT$", "w", stderr);

    SetConsoleTitleA("Phantom | Loading");

    MH_Initialize();

/*  Engine::Setup();*/

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
    FortMission_RiftSpawners::Setup();
    FortAthenaMutator_Barrier::Setup();
    AthenaBarrierFlag::Setup();
    FortPlayerControllerAthena::Setup();

    Utils::Hook(InSDKUtils::GetImageBase() + 0x196EEE0, ReturnTrue);
    Utils::Hook(InSDKUtils::GetImageBase() + 0x656320, ReturnTrue);

    Utils::Patch<uint8_t>(InSDKUtils::GetImageBase() + 0xC96B25 + 1, 0x85);

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