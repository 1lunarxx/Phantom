#pragma once
#include "pch.h"

template< class T >
static T* NewObject(UObject* Outer)
{
    return static_cast<T*>(UGameplayStatics::SpawnObject(T::StaticClass(), Outer));
}