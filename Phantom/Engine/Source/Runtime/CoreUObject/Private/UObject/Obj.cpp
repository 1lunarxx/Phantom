#include "pch.h"
#include "CoreUObject/Public/UObject/Object.h"

bool Object::CanCreateInCurrentContext(UObject* Template)
{
	return true;
}

void Object::Setup()
{
	Utils::Hook(InSDKUtils::GetImageBase() + 0x196EEE0, CanCreateInCurrentContext);
}