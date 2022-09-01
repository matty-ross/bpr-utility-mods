#include "../mem.h"


void Patch()
{
    // color override
    {
        static const uint8_t code[] = { 0xEB, 0x33 };
        PatchCode(0x06A6DF78, sizeof(code), code);
    }

    // palette type
    {
        RemoveCode(0x0769BE98, 18);
    }

    // color
    {
        RemoveCode(0x00B058BD, 20);
    }

    // paint shop
    {
        RemoveCode(0x00A1B485, 23);
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
