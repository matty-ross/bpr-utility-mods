#include "../mem.h"


void Patch()
{
    // allow any challenge type (except timed) to be selectable with any number of players
    {
        static const uint8_t code[] = { 0x83, 0xFE, 0x00, 0x74, 0x04 };
        PatchCode(0x076B1E5F, sizeof(code), code);
    }

    // pretend to be island challenge (allow any number of players)
    {
        static const uint8_t code[] = { 0xE9, 0xAE, 0x01, 0xC6, 0xF9, 0x90 };
        PatchCode(0x06D8367D, sizeof(code), code);
    }

    // don't increment the number of players who have left
    {
        RemoveCode(0x009E4570, 1);
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