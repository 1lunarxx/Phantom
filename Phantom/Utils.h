#pragma once
#include "pch.h"

class Utils
{
public:
	static void Hook(uintptr_t Address, void* Detour, void** Original = NULL)
	{
		MH_STATUS CreateHook_Status = MH_CreateHook((LPVOID)Address, Detour, (void**)Original);

		if (CreateHook_Status != MH_STATUS::MH_OK)
			printf("[PHANTOM] Failed to hook 0x%llX\n", (unsigned long long)Address);
		else
			MH_EnableHook((LPVOID)Address);
	}

    static void Virtual(void** VTable, uintptr_t Target, void* Detour, void** Original = NULL)
    {
        if (Original)
            *Original = VTable[(int)Target];

        DWORD dwProt;
        VirtualProtect(&VTable[(int)Target], sizeof(void*), PAGE_EXECUTE_READWRITE, &dwProt);

        VTable[(int)Target] = Detour;

        DWORD dwTemp;
        VirtualProtect(&VTable[(int)Target], sizeof(void*), dwProt, &dwTemp);
    }

    static void Virtual(UObject* Object, uintptr_t Target, void* Detour, void** Original = NULL)
    {
        for (int32 i = 0; i < UObject::GObjects->Num(); i++)
        {
            UObject* GObject = UObject::GObjects->GetByIndex(i);

            if (GObject == NULL)
                continue;

            if (GObject->IsA(Object->Class))
            {
                if (Original)
                    *Original = GObject->VTable[(int)Target];

                DWORD dwProt;
                VirtualProtect(&GObject->VTable[(int)Target], sizeof(void*), PAGE_EXECUTE_READWRITE, &dwProt);

                GObject->VTable[(int)Target] = Detour;

                DWORD dwTemp;
                VirtualProtect(&GObject->VTable[(int)Target], sizeof(void*), dwProt, &dwTemp);
            }
        }
    }

    template <typename _Is>
    static void Patch(uintptr_t Target, _Is Byte)
    {
        DWORD OldProtect;
        VirtualProtect(LPVOID(Target), sizeof(_Is), PAGE_EXECUTE_READWRITE, &OldProtect);

        *(_Is*)Target = Byte;
        VirtualProtect(LPVOID(Target), sizeof(_Is), OldProtect, &OldProtect);
    }

    template<typename T = UObject>
    static T* StaticFindObject(const TCHAR* OrigInName, UObject* InObjectPackage = NULL, UClass* ObjectClass = NULL)
    {
        static UObject* (*StaticFindObject)(UClass*, UObject*, const TCHAR*, bool) = decltype(StaticFindObject)(InSDKUtils::GetImageBase() + 0x19C8280);
        return (T*)StaticFindObject(ObjectClass, InObjectPackage, OrigInName, false);
    }

    static uint8_t* AllocateNearbyPage(void* targetAddr)
    {
        SYSTEM_INFO SysInfo;

        GetSystemInfo(&SysInfo);

        const uint64_t PageSize = SysInfo.dwPageSize;
        const uint64_t StartAddr = (uint64_t(targetAddr) & ~(PageSize - 1));
        const uint64_t MinAddr = min(StartAddr - 0x7FFFFF00, (uint64_t)SysInfo.lpMinimumApplicationAddress);
        const uint64_t MaxAddr = max(StartAddr + 0x7FFFFF00, (uint64_t)SysInfo.lpMaximumApplicationAddress);
        const uint64_t StartPage = (StartAddr - (StartAddr % PageSize));

        for (uint64_t PageOffset = 1; PageOffset; PageOffset++)
        {
            uint64_t ByteOffset = PageOffset * PageSize;
            uint64_t HighAddr = StartPage + ByteOffset;
            uint64_t LowAddr = (StartPage > ByteOffset) ? StartPage - ByteOffset : 0;

            bool NeedsExit = HighAddr > MaxAddr && LowAddr < MinAddr;

            if (HighAddr < MaxAddr)
            {
                if (void* OutAddr = VirtualAlloc((void*)HighAddr, PageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE))
                {
                    return (uint8_t*)OutAddr;
                }
            }

            if (LowAddr > MinAddr)
            {
                if (void* OutAddr = VirtualAlloc((void*)LowAddr, PageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE))
                {
                    return (uint8_t*)OutAddr;
                }
            }

            if (NeedsExit)
            {
                break;
            }
        }

        return NULL;
    }

    static void Rel32(uintptr_t Target, void* Detour, void** Original = nullptr)
    {
        uint8* Impl = (uint8*)(Target);
        uint8_t* NearPage = AllocateNearbyPage(Impl);

        if (NearPage == NULL)
            return;

        uint8_t Shellcode[] =
        {
            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        if (Detour != NULL)
        {
            memcpy(Shellcode + 6, &Detour, 8);
            memcpy(NearPage, Shellcode, sizeof(Shellcode));

            auto Offset = NearPage - (Impl + (int)5);

            memcpy(Impl + 1, &Offset, sizeof(int));
        }
        else
        {
            memset(Impl, 0x90, sizeof(int) + 1);
        }
    }
};

#define ANY_PACKAGE (UObject*)-1

template<class T>
static T* Cast(UObject* Object)
{
    return Object && (Object->IsA(T::StaticClass())) ? (T*)Object : NULL;
}