#include "mem.h"


struct Effects
{
    bool Bloom = false;
    bool Vignette = false;
    bool Contrast = false;
};


Effects g_Effects;


void Patch()
{
    if (g_Effects.Bloom)
    {
        static const uint8_t code[] = { 0xEB, 0x4A };
        PatchCode(0x00A5826B, sizeof(code), code);
    }

    if (g_Effects.Vignette)
    {
        static const uint8_t code[] = { 0xE9, 0xDF, 0x00, 0x00, 0x00, 0x90 };
        PatchCode(0x00A582E3, sizeof(code), code);
    }

    if (g_Effects.Contrast)
    {
        static const uint8_t code[] = { 0xE9, 0xF2, 0x00, 0x00, 0x00, 0x90 };
        PatchCode(0x070D006D, sizeof(code), code);
    }
}

void LoadEffects()
{
    const char* const fileName = ".\\mods\\effects\\config.ini";

    g_Effects.Bloom = GetPrivateProfileIntA("Config", "Bloom", 0, fileName);
    g_Effects.Vignette = GetPrivateProfileIntA("Config", "Vignette", 0, fileName);
    g_Effects.Contrast = GetPrivateProfileIntA("Config", "Contrast", 0, fileName);
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
        LoadEffects();
        Patch();
        break;

    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}
