#include <cstdio>

#include "../mem.h"


void __stdcall PrintMessage(const char* message)
{
    printf_s("%s", message);
}

void Patch()
{
    // default logger (points to an empty function)
    {
        const void* const newFunction = PrintMessage;
        PatchCode(0x00F0280C, sizeof(newFunction), &newFunction);
    }
}

BOOL WINAPI DllMain(
    _In_ HINSTANCE hinstDLL,
    _In_ DWORD     fdwReason,
    _In_ LPVOID    lpvReserved
)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hinstDLL);
        Patch();
        break;

    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}