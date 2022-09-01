#include "../mem.h"


void Patch()
{
    // crash check
    {
        static const uint8_t code[] = { 0xEB, 0x35 };
        PatchCode(0x06D3A241, sizeof(code), code);
    }

    // upper damage limit
    {
        RemoveCode(0x06D3A22F, 8);
    }

    // add time check
    {
        RemoveCode(0x009ECCA5, 6);
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