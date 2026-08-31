#pragma once
#include "pch.h"
#include "Utils.h"

class NetDriver
{
	class Originals
	{
	public:
		static inline void (*TickFlush)(UNetDriver* NetDriver, float DeltaSeconds);
	};

private:
	static void TickFlush(UNetDriver* NetDriver, float DeltaSeconds);
public:
	static void Setup();
};