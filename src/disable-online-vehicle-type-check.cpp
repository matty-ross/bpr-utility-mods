#include "mem.h"


void Patch()
{
    // GUI check
    {
        RemoveCode(0x00A2188D, 6);
    }

    // update check
    {
        static const uint8_t code[] = { 0xE9, 0xF8, 0x00, 0x00, 0x00, 0x90 };
        PatchCode(0x00A22A82, sizeof(code), code);
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
