#include <cstdio>

#include "../mem.h"


static constexpr char k_FileName[] = ".\\mods\\window-saved-position\\config.ini";


struct WindowPositionManager
{
    int PositionX = 0;
    int PositionY = 0;
    
    void Load()
    {
        PositionX = GetPrivateProfileIntA("Position", "X", 0, k_FileName);
        PositionY = GetPrivateProfileIntA("Position", "Y", 0, k_FileName);
    }

    void Save() const
    {
        char positionX[8] = {};
        sprintf_s(positionX, "%d", PositionX);
        WritePrivateProfileStringA("Position", "X", positionX, k_FileName);

        char positionY[8] = {};
        sprintf_s(positionY, "%d", PositionY);
        WritePrivateProfileStringA("Position", "Y", positionY, k_FileName);
    }

    void Update(HWND windowHandle)
    {
        WINDOWPLACEMENT windowPlacement =
        {
            .length = sizeof(WINDOWPLACEMENT),
        };
        GetWindowPlacement(windowHandle, &windowPlacement);

        PositionX = windowPlacement.rcNormalPosition.left;
        PositionY = windowPlacement.rcNormalPosition.top;
    }
};


WindowPositionManager g_WindowPositionManager;


void __stdcall OverwriteWindowPosition(int* positionX, int* positionY)
{
    *positionX = g_WindowPositionManager.PositionX;
    *positionY = g_WindowPositionManager.PositionY;
}

__declspec(naked) void PatchWindowCreation()
{
    __asm
    {
        pushfd
        pushad

        lea eax, [esi + 0x18]
        push eax
        lea eax, [esi + 0x1C]
        push eax
        call OverwriteWindowPosition

        popad
        popfd

        // original code
        push dword ptr [esi + 0x0]
        push dword ptr [esi + 0x4]
        push 0x0817799A
        ret
    }
}

__declspec(naked) void PatchDirect3DInitialization()
{
    __asm
    {
        pushfd
        pushad

        lea eax, [ebp - 0x28]
        push eax
        lea eax, [ebp - 0x2C]
        push eax
        call OverwriteWindowPosition

        popad
        popfd

        // original code
        mov edi, dword ptr [ebp - 0x2C]
        push 0x08187FEF
        ret
    }
}

__declspec(naked) void PatchChangeResolution()
{
    __asm
    {
        pushfd
        pushad

        lea eax, [esp + 0x94]
        push eax
        lea eax, [esp + 0x94]
        push eax
        call OverwriteWindowPosition

        popad
        popfd

        // original code
        push 0x20
        push dword ptr [esp + 0x30]
        push 0x0817E12E
        ret
    }
}

DWORD CALLBACK SaveWindowPositionThreadProc(LPVOID lpParameter)
{
    while (true)
    {
        HWND windowHandle = *reinterpret_cast<HWND*>(0x0139815C);
        if (windowHandle != NULL)
        {
            g_WindowPositionManager.Update(windowHandle);
            g_WindowPositionManager.Save();
        }

        Sleep(5000);
    }

    return 0;
}


void Patch()
{
    // window creation
    {
        static uint8_t code[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
        *reinterpret_cast<uint32_t*>(code + 0x1) = reinterpret_cast<uint32_t>(PatchWindowCreation) - 0x08177995 - 0x5;
        PatchCode(0x08177995, sizeof(code), code);
    }

    // Direct3D initialization
    {
        static uint8_t code[] = { 0xE9, 0x00, 0x00, 0x00, 0x00, 0x90 };
        *reinterpret_cast<uint32_t*>(code + 0x1) = reinterpret_cast<uint32_t>(PatchDirect3DInitialization) - 0x08187FE9 - 0x5;
        PatchCode(0x08187FE9, sizeof(code), code);
    }

    // change resolution
    {
        static uint8_t code[] = { 0xE9, 0x00, 0x00, 0x00, 0x00, 0x90 };
        *reinterpret_cast<uint32_t*>(code + 0x1) = reinterpret_cast<uint32_t>(PatchChangeResolution) - 0x0817E128 - 0x5;
        PatchCode(0x0817E128, sizeof(code), code);
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
        g_WindowPositionManager.Load();
        CreateThread(nullptr, 0, SaveWindowPositionThreadProc, nullptr, 0, nullptr);
        Patch();
        break;

    case DLL_PROCESS_DETACH:
        g_WindowPositionManager.Save();
        break;
    }

    return TRUE;
}