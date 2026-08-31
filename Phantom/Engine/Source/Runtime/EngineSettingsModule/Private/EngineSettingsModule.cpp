#include "pch.h"
#include "Utils.h"

void UGameMapsSettings::SetGameDefaultMap(const FString& NewMap)
{
	UGameMapsSettings* GameMapsSettings = Cast<UGameMapsSettings>(UGameMapsSettings::StaticClass()->DefaultObject);

/*	if (IsRunningDedicatedServer())
	{
		GameMapsSettings->ServerDefaultMap = NewMap;
	}
	else
	{
		GameMapsSettings->GameDefaultMap = NewMap;
	}*/

	FSoftObjectPath* GameDefaultMap = (FSoftObjectPath*)((uintptr_t)GameMapsSettings + 0x98);

	GameDefaultMap->AssetPathName = UKismetStringLibrary::Conv_StringToName(NewMap);
	GameDefaultMap->SubPathString = FString();
}