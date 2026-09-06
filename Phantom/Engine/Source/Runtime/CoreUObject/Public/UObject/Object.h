#pragma once
#include "pch.h"
#include "Utils.h"

class Object
{
private:
	static bool CanCreateInCurrentContext(UObject* Template);
public:
	static void Setup();
};