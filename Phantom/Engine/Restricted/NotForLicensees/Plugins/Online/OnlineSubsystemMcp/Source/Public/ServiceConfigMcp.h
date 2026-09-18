#pragma once
#include "pch.h"
#include "Utils.h"

class FServicePermissionsMcp
{
public:
	FString Name;
	FString Id;
	FString Key;
};

class FServiceConfigMcp
{
private:
	static FServicePermissionsMcp* GetServicePermissionsById(FServiceConfigMcp* ServiceConfigMcp, const FString* Id);
	static FServicePermissionsMcp* GetServicePermissionsByName(FServiceConfigMcp* ServiceConfigMcp, const FString* Id);
public:
	static void Setup();
};