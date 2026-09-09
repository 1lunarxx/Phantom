#pragma once
#include "pch.h"
#include "Core/Public/Misc/OutputDevice.h"

struct FFrame : public FOutputDevice
{
public:
    UFunction* Node;
    UObject* Object;
    uint8* Code;
    uint8* Locals;
    void* MostRecentProperty;
    uint8_t* MostRecentPropertyAddress;
    uint8_t _Padding1[0x40];
    UField* PropertyChainForCompiledIn;
public:

    // Functions.
    void Step(UObject* Context, void* const RESULT_DECL);

    /** Replacement for Step that uses an explicitly specified property to unpack arguments **/
    void StepExplicitProperty(void* const Result, const UField* Property);

    /** Replacement for Step that checks the for byte code, and if none exists, then PropertyChainForCompiledIn is used. **/
    FORCEINLINE void StepCompiledIn(void* const Result = NULL);

    /** Replacement for Step that checks the for byte code, and if none exists, then PropertyChainForCompiledIn is used. **/
    template<typename TNativeType>
    FORCEINLINE TNativeType& StepCompiledInRef();

    void IncrementCode()
    {
        Code += !!Code;
    }
};

FORCEINLINE void FFrame::StepCompiledIn(void* const Result)
{
    if (Code)
    {
        Step(Object, Result);
    }
    else
    {
        const UField* Prop = *(const UField**)(__int64(this) + 0x80);

        if (Prop != NULL)
        {
            *(const UField**)(__int64(this) + 0x80) = *(const UField**)(__int64(Prop) + 0x28);

            StepExplicitProperty(Result, Prop);
        }
    }
}

template<typename TNativeType>
FORCEINLINE TNativeType& FFrame::StepCompiledInRef()
{
    static TNativeType TemporaryBuffer{};

    MostRecentPropertyAddress = NULL;

    if (Code)
    {
        Step(Object, &TemporaryBuffer);
    }
    else
    {
        UField* Property = *(UField**)(__int64(this) + 0x80);

        if (Property != NULL)
        {
            *(const UField**)(__int64(this) + 0x80) = *(const UField**)(__int64(Property) + 0x28);

            StepExplicitProperty(&TemporaryBuffer, Property);
        }
    }

    return (MostRecentPropertyAddress != NULL) ? *(TNativeType*)(MostRecentPropertyAddress) : TemporaryBuffer;
}