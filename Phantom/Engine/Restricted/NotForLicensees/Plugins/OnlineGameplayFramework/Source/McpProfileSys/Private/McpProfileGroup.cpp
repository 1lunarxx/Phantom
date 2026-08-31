#include "pch.h"
#include "Engine/Restricted/NotForLicensees/Plugins/OnlineGameplayFramework/Source/McpProfileSys/Public/McpProfileGroup.h"

void McpProfileGroup::SendRequestNow(UMcpProfileGroup* McpProfileGroup, const void* HttpRequest)
{
	*(int*)(__int64(HttpRequest) + 0x28) = 3;

	Originals::SendRequestNow(McpProfileGroup, HttpRequest);
}

void McpProfileGroup::Setup()
{
	Utils::Hook(InSDKUtils::GetImageBase() + 0x96C910, SendRequestNow, (void**)&Originals::SendRequestNow);
}