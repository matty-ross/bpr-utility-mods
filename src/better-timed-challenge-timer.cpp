#include "../mem.h"


static constexpr const char* k_Format = "%d:%05.2f";


__declspec(naked) void CustomFormat()
{
    __asm
    {
        cvtps2pd xmm1, xmm1
        sub esp, 0x8
        movsd [esp], xmm1
        push ecx
        push dword ptr [k_Format]
        jmp eax
    }
}


void Patch()
{
    // target
    {
        uint8_t code[] = { 0xB8, 0x00, 0x00, 0x00, 0x00, 0xE9, 0x00, 0x00, 0x00, 0x00, 0x90 };
        *reinterpret_cast<uintptr_t*>(code + 0x1) = 0x00A84E07;
        *reinterpret_cast<uint32_t*>(code + 0x6) = reinterpret_cast<uint32_t>(CustomFormat) - 0x00A84E01 - 0x5;
        PatchCode(0x00A84DFC, sizeof(code), code);
    }

    // personal best
    {
        uint8_t code[] = { 0xB8, 0x00, 0x00, 0x00, 0x00, 0xE9, 0x00, 0x00, 0x00, 0x00, 0x90 };
        *reinterpret_cast<uintptr_t*>(code + 0x1) = 0x00A84E8B;
        *reinterpret_cast<uint32_t*>(code + 0x6) = reinterpret_cast<uint32_t>(CustomFormat) - 0x00A84E85 - 0x5;
        PatchCode(0x00A84E80, sizeof(code), code);
    }

    // time
    {
        uint8_t code[] = { 0xB8, 0x00, 0x00, 0x00, 0x00, 0xE9, 0x00, 0x00, 0x00, 0x00, 0x90 };
        *reinterpret_cast<uintptr_t*>(code + 0x1) = 0x00A84F12;
        *reinterpret_cast<uint32_t*>(code + 0x6) = reinterpret_cast<uint32_t>(CustomFormat) - 0x00A84F0C - 0x5;
        PatchCode(0x00A84F07, sizeof(code), code);
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