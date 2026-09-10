#pragma once
#include "pch.h"

template< class T >
static T* NewObject(UObject* Outer, UClass* InClass = NULL)
{
    return static_cast<T*>(UGameplayStatics::SpawnObject(InClass != NULL ? InClass : T::StaticClass(), Outer));
}