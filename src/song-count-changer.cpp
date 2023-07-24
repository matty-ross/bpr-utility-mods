#include "../mem.h"


int32_t g_SongCount = 90;


void Patch()
{
    {
        static uint32_t bits = 0xFFFFFFFF;
        
        PatchCode(0x07238997, sizeof(bits), &bits);
        PatchCode(0x072389A2, sizeof(bits), &bits);
        RemoveCode(0x072389B9, 7);
    }

    {
        static uint32_t bits = 0xFFFFFFFF;

        PatchCode(0x072389E0, sizeof(bits), &bits);
        PatchCode(0x072389EB, sizeof(bits), &bits);
        RemoveCode(0x07238A16, 7);
    }

    {
        static uint8_t code[] = { 0xE9, 0x48, 0x01, 0x00, 0x00, 0x90 };
        PatchCode(0x004F716C, sizeof(code), code);
    }

    {
        PatchCode(0x07238A08, sizeof(g_SongCount), &g_SongCount);
    }
}

void LoadSongCount()
{
    const char* const fileName = ".\\mods\\song-count-changer\\config.ini";

    g_SongCount = GetPrivateProfileIntA("Config", "SongCount", 90, fileName);
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
        LoadSongCount();
        Patch();
        break;

    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}