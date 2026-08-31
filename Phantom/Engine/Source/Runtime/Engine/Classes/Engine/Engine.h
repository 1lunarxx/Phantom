#pragma once
#include "pch.h"
#include "Utils.h"

class Engine
{
	class Originals
	{
	public:
		static inline void (*Init)(UEngine* Engine, void* InEngineLoop);
	};

private:
	static void Init(UEngine* Engine, void* InEngineLoop);
public:
	static void Setup();
};