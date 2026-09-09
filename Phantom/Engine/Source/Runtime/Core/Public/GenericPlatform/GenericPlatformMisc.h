#pragma once
#include "pch.h"

struct FGenericPlatformMisc
{
public:
	static void RequestExit(bool Force)
	{
		static void(*RequestExit)(bool) = decltype(RequestExit)(InSDKUtils::GetImageBase() + 0x18066E0);
		RequestExit(Force);
	}
};