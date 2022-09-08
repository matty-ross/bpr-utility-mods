#include "../mem.h"


__declspec(naked) void SelectButtonCheck()
{
    __asm
    {
        // check whether the challenge is timed
        cmp dword ptr [edx], 0
        je original_code

        // make other challenges selectable
        mov byte ptr [ecx + edx + 0x2B90D], 1
        mov byte ptr [ecx + edx + 0x2B90C], 1
        jmp end

    original_code:
        cmp [ecx + edx + 0x2B90D], al
        je end
        mov [ecx + edx + 0x2B90D], al
        mov byte ptr [ecx + edx + 0x2B90C], 1

    end:
        ret
    }
}

void Patch()
{
    // always selectable
    {
        const uint8_t code = 0xE9;
        PatchCode(0x076B1E71, sizeof(code), &code);
        
        const uintptr_t dest = reinterpret_cast<uintptr_t>(SelectButtonCheck) - 0x076B1E71 - 0x5;
        PatchCode(0x076B1E72, sizeof(dest), &dest);
        
        RemoveCode(0x076B1E76, 2);
    }

    // players count check
    {
        static const uint8_t code[] = { 0xE9, 0xAE, 0x01, 0xC6, 0xF9, 0x90 };
        PatchCode(0x06D8367D, sizeof(code), code);
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