#include "mem.h"


static float g_Time = 0.0f;


__declspec(naked) void SaveTime()
{
    static const float factor = 1000.0f;

    __asm
    {
        // convert the time from float to int
        movss xmm0, xmm5
        mulss xmm0, dword ptr [factor]
        cvtss2si eax, xmm0

        // get the last 3 digits
        pushad
        xor edx, edx
        mov ebx, 1000
        div ebx
        mov dword ptr [g_Time], edx
        popad

        // original code
        mov [esp + 0x14], ecx
        xorps xmm0, xmm0
        push 0x0621B567
        ret
    }
}

__declspec(naked) void UseTime()
{
    __asm
    {
        mov eax, dword ptr [g_Time]

        // original code
        lea ecx, [esp + 0x48]
        push 0x0621B701
        ret
    }
}

void Patch()
{
    // save the time
    {
        uint8_t code[] = { 0xE9, 0x00, 0x00 , 0x00 , 0x00, 0x90, 0x90 };
        *reinterpret_cast<uint32_t*>(code + 0x1) = reinterpret_cast<uint32_t>(SaveTime) - 0x0621B560 - 0x5;
        PatchCode(0x0621B560, sizeof(code), code);
    }

    // use time
    {
        uint8_t code[] = { 0xE9, 0x00, 0x00 , 0x00 , 0x00, 0x90 };
        *reinterpret_cast<uint32_t*>(code + 0x1) = reinterpret_cast<uint32_t>(UseTime) - 0x0621B6FB - 0x5;
        PatchCode(0x0621B6FB, sizeof(code), code);
    }
    
    // format - 3 decimal places
    {
        static const uint8_t code[] = { 0x03 };
        PatchCode(0x0621B705, sizeof(code), code);
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
