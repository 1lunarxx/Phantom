#include "pch.h"
#include "Engine/Engine.h"

void Engine::Init(UEngine* Engine, void* InEngineLoop)
{
	UGameMapsSettings::SetGameDefaultMap(L"/Game/Athena/Maps/Athena_Terrain.Athena_Terrain");
	Originals::Init(Engine, InEngineLoop);
}

void Engine::Setup()
{
	Utils::Hook(InSDKUtils::GetImageBase() + 0xEF56C0, Init, (void**)&Originals::Init);
}