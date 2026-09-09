#pragma once
#include "pch.h"

class FOutputDevice
{
public:
    void** VTable;
    bool bSuppressEventTag;
    bool bAutoEmitLineTerminator;
};