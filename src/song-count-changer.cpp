#include "mem.h"


int32_t g_SongCount = 90;


void Patch()
{
    PatchCode(0x07238A08, sizeof(g_SongCount), &g_SongCount);
    PatchCode(0x00BE40F3, sizeof(g_SongCount), &g_SongCount);
    //PatchCode(0x00A256E3, sizeof(g_SongCount), &g_SongCount);
    //PatchCode(0x03943B1C, sizeof(g_SongCount), &g_SongCount);
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
