#include "../mem.h"


struct TraxData
{
    int32_t SongCount = 90;
    int32_t ParadiseCityIndex = 24;
    int32_t FirstClassicalMusicIndex = 68;
};


TraxData g_TraxData;


void Patch()
{
    // song count
    {
        PatchCode(0x07238A08, 4, &g_TraxData.SongCount);
        PatchCode(0x00BE40F3, 4, &g_TraxData.SongCount);
    }
    
    // paradise city index
    {
        PatchCode(0x00BE5100, 4, &g_TraxData.ParadiseCityIndex);
        PatchCode(0x00BE5193, 1, &g_TraxData.ParadiseCityIndex);
        PatchCode(0x00BE51F0, 4, &g_TraxData.ParadiseCityIndex);
    }

    // first classical music index
    {
        PatchCode(0x00916168, 1, &g_TraxData.FirstClassicalMusicIndex);
    }
}

void LoadTraxData()
{
    const char* const fileName = ".\\mods\\song-count-changer\\config.ini";

    g_TraxData.SongCount = GetPrivateProfileIntA("Config", "SongCount", 90, fileName);
    g_TraxData.ParadiseCityIndex = GetPrivateProfileIntA("Config", "ParadiseCityIndex", 24, fileName);
    g_TraxData.FirstClassicalMusicIndex = GetPrivateProfileIntA("Config", "FirstClassicalMusicIndex", 68, fileName);
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
        LoadTraxData();
        Patch();
        break;

    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}