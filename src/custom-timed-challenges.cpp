#include "../mem.h"


void Patch()
{
    // skip time limit check
    {
        static const uint8_t code[] = { 0xEB, 0x21 };
        PatchCode(0x009FECD8, sizeof(code), code);
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