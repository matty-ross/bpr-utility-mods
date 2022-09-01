#include "mem.h"


void Patch()
{
    // always selectable
    {
        static const uint8_t code[] = { 0xC6, 0x84, 0x11, 0x0D, 0xB9, 0x02, 0x00, 0x01 };
        PatchCode(0x076B1E71, sizeof(code), code);
        RemoveCode(0x076B1E79, 8);
    }

    // players count check
    {
        static const uint8_t code[] = { 0xE9, 0xAE, 0x01, 0xC6, 0xF9, 0x90 };
        PatchCode(0x06D8367D, sizeof(code), code);
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
