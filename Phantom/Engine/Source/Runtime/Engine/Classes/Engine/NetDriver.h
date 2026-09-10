#pragma once
#include "pch.h"
#include "Utils.h"

class NetDriver
{
	class Originals
	{
	public:
		static inline void (*TickFlush)(UNetDriver* NetDriver, float DeltaSeconds);
		static inline UReplicationDriver* (*ConditionalCreateReplicationDriver)(UNetDriver* ForNetDriver, UWorld* World);
	};

private:
	static void TickFlush(UNetDriver* NetDriver, float DeltaSeconds);
	static UReplicationDriver* ConditionalCreateReplicationDriver(UNetDriver* ForNetDriver, UWorld* World);
public:
	static void Setup();
};