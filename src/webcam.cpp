#include "mem.h"


// Save offset: 0x91C86


void Patch()
{
    // always read camera option 'off'
    {
        static const uint8_t code[] = { 0x31, 0xC0 };
        PatchCode(0x00A3CD84, sizeof(code), code);
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
