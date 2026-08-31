#pragma once
#include "pch.h"
#include "Utils.h"

class McpProfileGroup
{
	class Originals
	{
	public:
		static inline void (*SendRequestNow)(UMcpProfileGroup* McpProfileGroup, const void* HttpRequest);
	};

private:
	static void SendRequestNow(UMcpProfileGroup* McpProfileGroup, const void* HttpRequest);
public:
	static void Setup();
};