// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.

FName::FName(FString String)
{
	*this = UKismetStringLibrary::Conv_StringToName(String);
}