#include "mem.h"


__declspec(naked) void CaseVoipOutputDevice()
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

__declspec(naked) void CaseVoipOutputDeviceOptionChange()
{
    __asm
    {
        mov ecx, esi
        mov dword ptr [esi + 0x33F24], eax
        mov eax, 0x00ACEA70
        call eax
        mov byte ptr [esi + 0x341EC], 1
        pop edi
        pop esi
        pop ecx
        ret
    }
}

__declspec(naked) void LoadSavedVoipOutputDevice()
{
    __asm
    {
        push ebp
        mov ebp, esp
        cmp dword ptr [ecx + 0xDC], 2
        jne _end
        mov edx, 0x0135AEDC
        push esi
        lea esi, [edx + 0x1]
    _strlen_continue:
        mov al, byte ptr [edx]
        inc edx
        test al, al
        jne _strlen_continue
        sub edx, esi
        lea eax, [ecx + 0x21]
        push edx
        push 0x0135AEDC
        push 0x20
        push eax
        call dword ptr ds:[0x08282184]
        mov eax, dword ptr [ebp + 0x8]
        add esp, 0x10
        pop esi
        push 0
        test eax, eax
        jns _arg_positive
        push 0
        push 0x636F6465
        mov eax, 0x005430B0
        call eax
        push eax
        mov eax, 0x00547AA0
        call eax
        add esp, 0x10
        pop ebp
        ret 4
    _arg_positive:
        push eax
        push 0x6F646576
        mov eax, 0x005430B0
        call eax
        push eax
        mov eax, 0x00547AA0
        call eax
        add esp, 0x10
    _end:
        pop ebp
        ret 4
    }
}

__declspec(naked) void HookLoadSavedVoipOutputDevice()
{
    __asm
    {
        mov eax, dword ptr [edi + 0x4]
        cmp eax, -2
        je _end
        mov ecx, dword ptr [ebx + 0x4]
        push eax
        add ecx, 0x3370
        call LoadSavedVoipOutputDevice
    _end:
        mov eax, 0x00B55CF7
        jmp eax
    }
}

__declspec(naked) void HookInitVoipOutputDevice()
{
    __asm
    {
        mov eax, dword ptr [ecx + 0x210]
        inc eax
        mov dword ptr [ecx + 0x274], eax

        // original code
        lea eax, [ecx + 0x24]
        mov dword ptr [ecx + 0x214], 0x00F03B9C
        
        push 0x032D8849
        ret
    }
}

__declspec(naked) void HookUpdateAudioSettings()
{
    __asm
    {
        mov eax, dword ptr [esi + 0x274]
        test eax, eax
        jnz _not_none
        push 0x00D96604
        jmp _continue
    _not_none:
        shl eax, 5
        add eax, 0xE4
        add eax, esi
        push eax
    _continue:
        push 0x20
        push 0x0135AEDC
        call dword ptr ds:[0x08282170]
        add esp, 0xC

        // original code
        mov eax, 0x00882700
        call eax
        mov eax, 0x02F30B6C
        jmp eax
    }
}

__declspec(naked) void HookSaveSettings()
{
    __asm
    {
        lea eax, [esp + 0xB0]
        push eax
        push 0x0135AEDC
        push 0x00D7FCCC
        push 0x00D7FFF0
        call edi

        // original code
        xor esi, esi
        mov eax, 0x060C1760
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

    // exclude VOIP output device from menu options when offline
    {
        uint32_t menuOptionIndex = 5;
        PatchCode(0x076DC268, sizeof(menuOptionIndex), &menuOptionIndex);
    }

    // BUGFIX: get VOIP output device
    {
        uint32_t identifier = 0x6F646576;
        PatchCode(0x0624ECA6, sizeof(identifier), &identifier);
    }

    // add a case with VOIP output device
    {
        uint32_t caseAddress = reinterpret_cast<uint32_t>(CaseVoipOutputDevice);
        PatchCode(0x00B0E828, sizeof(caseAddress), &caseAddress);
    }

    // change case for VOIP output device option change
    {
        uint32_t caseAddress = reinterpret_cast<uint32_t>(CaseVoipOutputDeviceOptionChange);
        PatchCode(0x00ACEBE4, sizeof(caseAddress), &caseAddress);
    }

    // hook load saved VOIP settings
    {
        uint8_t code[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
        *reinterpret_cast<uint32_t*>(code + 0x1) = reinterpret_cast<uint32_t>(HookLoadSavedVoipOutputDevice) - 0x00B55F1A - 0x5;
        PatchCode(0x00B55F1A, sizeof(code), code);
    }

    // hook init audio settings screen
    {
        uint8_t code[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
        *reinterpret_cast<uint32_t*>(code + 0x1) = reinterpret_cast<uint32_t>(HookInitVoipOutputDevice) - 0x032D883C - 0x5;
        PatchCode(0x032D883C, sizeof(code), code);
        RemoveCode(0x032D8841, 8);
    }

    // hook update audio settings
    {
        uint8_t code[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
        *reinterpret_cast<uint32_t*>(code + 0x1) = reinterpret_cast<uint32_t>(HookUpdateAudioSettings) - 0x02F30B67 - 0x5;
        PatchCode(0x02F30B67, sizeof(code), code);
    }

    // hook save settings
    {
        uint8_t code[] = { 0xE9, 0x00, 0x00, 0x00, 0x00, 0x90 };
        *reinterpret_cast<uint32_t*>(code + 0x1) = reinterpret_cast<uint32_t>(HookSaveSettings) - 0x060C175A - 0x5;
        PatchCode(0x060C175A, sizeof(code), code);
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
