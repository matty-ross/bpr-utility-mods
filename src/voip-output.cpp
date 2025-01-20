#include "mem.h"


__declspec(naked) void CaseVoipOutput()
{
    static constexpr char menuLabel[] = "$OPTIONS_MENU_VOIP_OUPUT";
    
    __asm
    {
        mov ecx, dword ptr [esp + 0x14]
        push 0
        lea eax, [ecx + 0x238]
        push eax
        push offset menuLabel
        push 0
        push dword ptr [ecx + 0x26C]
        mov ecx, ebx
        push esi
        mov eax, 0x004F63A0
        call eax

        mov ecx, dword ptr [esp + 0x14]
        cmp dword ptr [ecx + 0x274], 0
        jnl _not_min
        mov dword ptr [ecx + 0x274], 0
    _not_min:
        mov eax, dword ptr [ecx + 0x208]
        test eax, eax
        jg _continue
        mov eax, 0x00B0E7B2
        jmp eax
    _continue:
        cmp dword ptr [ecx + 0x274], eax
        jle _not_max
        mov dword ptr [ecx + 0x274], eax
    _not_max:
        push dword ptr [ecx + 0x274]
        mov eax, 0x00B0E7AA
        jmp eax
    }
}


void Patch()
{
    // change the number of menu items
    {
        uint32_t numMenuItems = 6;
        PatchCode(0x00B0E38E, sizeof(numMenuItems), &numMenuItems);
    }

    // change switch guard
    {
        uint8_t switchGuard = 5;
        PatchCode(0x00B0E404, sizeof(switchGuard), &switchGuard);
    }

    // change the number of iterations
    {
        uint8_t numIterations = 6;
        PatchCode(0x00B0E801, sizeof(numIterations), &numIterations);
    }

    // add a case
    {
        uint32_t caseAddress = reinterpret_cast<uint32_t>(CaseVoipOutput);
        PatchCode(0x00B0E828, sizeof(caseAddress), &caseAddress);
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
