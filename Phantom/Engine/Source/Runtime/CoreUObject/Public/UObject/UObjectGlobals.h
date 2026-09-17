#pragma once
#include "pch.h"
#include "ObjectMacros.h"
#include "Core/Public/UObject/UnrealNames.h"

#define ANY_PACKAGE (UObject*)-1

FORCEINLINE UPackage* GetTransientPackage()
{
	static UPackage* (*GetTransientPackage)() = decltype(GetTransientPackage)(InSDKUtils::GetImageBase() + 0x197BC10);
	return GetTransientPackage();
}

FORCEINLINE UObject* StaticConstructObject_Internal(UClass* InClass, UObject* InOuter, FName InName, EObjectFlags InFlags, EInternalObjectFlags InternalSetFlags, UObject* InTemplate, bool bCopyTransientsFromClassDefaults, void* InInstanceGraph, bool bAssumeTemplateIsArchetype)
{
	static UObject* (*StaticConstructObject_Internal)(UClass*, UObject*, FName, EObjectFlags, EInternalObjectFlags, UObject*, bool, void*, bool) = decltype(StaticConstructObject_Internal)(InSDKUtils::GetImageBase() + 0x19C7430);
	return StaticConstructObject_Internal(InClass, InOuter, InName, InFlags, InternalSetFlags, InTemplate, bCopyTransientsFromClassDefaults, InInstanceGraph, bAssumeTemplateIsArchetype);
}

template<class T>
T* NewObject(UObject* Outer = (UObject*)GetTransientPackage(), UClass* Class = NULL)
{
	return static_cast<T*>(StaticConstructObject_Internal(Class != NULL ? Class : T::StaticClass(), Outer, NAME_None, EObjectFlags::NoFlags, EInternalObjectFlags::None, nullptr, false, nullptr, false));
}