#include "pch.h"
#include "Engine/Restricted/NotForLicensees/Plugins/Online/OnlineSubsystemMcp/Source/Public/ServiceConfigMcp.h"

FServicePermissionsMcp* FServiceConfigMcp::GetServicePermissionsById(FServiceConfigMcp* ServiceConfigMcp, const FString* Id)
{
    TArray<FServicePermissionsMcp>& ServicePermissions = *(TArray<FServicePermissionsMcp>*)(__int64(ServiceConfigMcp) + 0x98);

    return &ServicePermissions[0];
}

FServicePermissionsMcp* FServiceConfigMcp::GetServicePermissionsByName(FServiceConfigMcp* ServiceConfigMcp, const FString* Id)
{
    TArray<FServicePermissionsMcp>& ServicePermissions = *(TArray<FServicePermissionsMcp>*)(__int64(ServiceConfigMcp) + 0x98);

    return &ServicePermissions[0];
}

void FServiceConfigMcp::Setup()
{
    Utils::Hook(InSDKUtils::GetImageBase() + 0x615230, GetServicePermissionsById);
    Utils::Hook(InSDKUtils::GetImageBase() + 0x615310, GetServicePermissionsByName);
}