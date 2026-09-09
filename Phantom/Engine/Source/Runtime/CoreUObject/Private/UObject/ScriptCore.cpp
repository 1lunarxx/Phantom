#include "pch.h"
#include "CoreUObject/Public/UObject/Stack.h"

void FFrame::Step(UObject* Context, void* const RESULT_DECL)
{
	static void(*Step)(FFrame*, UObject*, void* const) = decltype(Step)(InSDKUtils::GetImageBase() + 0x19A35F0);
	Step(this, Context, RESULT_DECL);
}

void FFrame::StepExplicitProperty(void* const Result, const UField* Property)
{
	static void(*StepExplicitProperty)(FFrame*, void* const, const UField*) = decltype(StepExplicitProperty)(InSDKUtils::GetImageBase() + 0x19A3620);
	StepExplicitProperty(this, Result, Property);
}