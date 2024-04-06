#include "mem.h"


// Save offset: 0x91C86


__declspec(naked) void ReadOption()
{
    __asm
    {
        mov eax, dword ptr [edx + 0x7768]
        mov [ecx + 0x4], eax
        ret
    }
}

__declspec(naked) void WriteOption()
{
    __asm
    {
        mov eax, dword ptr [ecx + 0x4]
        mov [edx + 0x7768], eax
        ret
    }
}

void Patch()
{
    // read the webcam option
    {
        uint8_t code[] = { 0xE8, 0x00, 0x00, 0x00, 0x00, 0x90, 0x90 };
        *reinterpret_cast<uint32_t*>(code + 0x1) = reinterpret_cast<uint32_t>(ReadOption) - 0x02F375C6 - 0x5;
        PatchCode(0x02F375C6, sizeof(code), code);
    }

    // write the webcam option
    {
        uint8_t code[] = { 0xE8, 0x00, 0x00, 0x00, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90 };
        *reinterpret_cast<uint32_t*>(code + 0x1) = reinterpret_cast<uint32_t>(WriteOption) - 0x02F38636 - 0x5;
        PatchCode(0x02F38636, sizeof(code), code);
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
