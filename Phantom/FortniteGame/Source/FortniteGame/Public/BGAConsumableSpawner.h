#pragma once
#include "pch.h"
#include "Utils.h"

class BGAConsumableSpawner
{
	class Originals
	{
	public:
		static inline void (*BeginPlay)(ABGAConsumableSpawner* BGAConsumableSpawner);
	};

private:
	static void BeginPlay(ABGAConsumableSpawner* BGAConsumableSpawner);
public:
	static void Setup();
};