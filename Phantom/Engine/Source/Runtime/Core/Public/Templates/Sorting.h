#pragma once
#include "pch.h"

// yes its ass

template<class T, class PREDICATE_CLASS>
void Sort(T* First, const int32 Num, const PREDICATE_CLASS& Predicate)
{
	for (int32 i = 1; i < Num; i++)
	{
		T Value = First[i];
		int32 j = i - 1;

		while (j >= 0 && Predicate(*Value, *First[j]))
		{
			First[j + 1] = First[j];
			j--;
		}

		First[j + 1] = Value;
	}
}