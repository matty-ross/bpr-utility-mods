#include "../mem.h"


struct MotionBlur
{
    float Vehicle = 0.0;
    float Environment = 0.0f;
};


MotionBlur g_MotionBlur;


void Patch()
{
    RemoveCode(0x03014D0F, 5);
    PatchCode(0x06833CE9, sizeof(float), &g_MotionBlur.Vehicle);
    PatchCode(0x06833CF0, sizeof(float), &g_MotionBlur.Environment);
}

void LoadMotionBlur()
{
    const char* const fileName = ".\\mods\\motion-blur\\config.ini";

    const auto readFloat = [=](const char* keyName) -> float
    {
        char buffer[32] = {};
        GetPrivateProfileStringA("Config", keyName, "0.0", buffer, sizeof(buffer), fileName);
        return static_cast<float>(atof(buffer));
    };

    g_MotionBlur.Vehicle = readFloat("Vehicle");
    g_MotionBlur.Environment = readFloat("Environment");
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
        LoadMotionBlur();
        Patch();
        break;

    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}