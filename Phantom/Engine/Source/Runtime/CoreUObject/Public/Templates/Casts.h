#pragma once
#include "pch.h"

template<class T>
static T* Cast(UObject* Object)
{
    return Object && Object->IsA(T::StaticClass()) ? (T*)Object : NULL;
}