#include "pch.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Engine/Restricted/NotForLicensees/Plugins/OnlineGameplayFramework/Source/McpProfileSys/Public/McpProfileGroup.h"
#include "Engine/NetDriver.h"

#include "FortniteGame/Public/FortGameState.h"
#include "FortniteGame/Public/Athena/FortGameModeAthena.h"
#include "FortniteGame/Public/Online/FortGameSession.h"
#include "FortniteGame/Public/Player/FortPlayerControllerZone.h"
#include "FortniteGame/Public/Player/FortPlayerController.h"

DWORD WINAPI LaunchWindowsStartup(LPVOID)
{
    AllocConsole();

    FILE* F = NULL;

    freopen_s(&F, "CONIN$", "r", stdin);
    freopen_s(&F, "CONOUT$", "w", stdout);
    freopen_s(&F, "CONOUT$", "w", stderr);

    SetConsoleTitleA("Phantom | Loading");

    MH_Initialize();

    Engine::Setup();

    World::Setup();
    FortGameState::Setup();
    FortGameModeAthena::Setup();
    McpProfileGroup::Setup();
    FortGameSession::Setup();
    NetDriver::Setup();
    FortPlayerControllerZone::Setup();
    FortPlayerController::Setup();

    Utils::Patch<uint8_t>(InSDKUtils::GetImageBase() + 0xCFD880, 0xC3);

    *GIsClient = false;
    *GIsServer = true;

    GWorld->ServerTravel(L"Athena_Terrain", false, false);
    GWorld->OwningGameInstance->RemoveLocalPlayer(GWorld->OwningGameInstance->LocalPlayers[0]);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        CreateThread(NULL, 0, LaunchWindowsStartup, NULL, 0, NULL);

    return TRUE;
}