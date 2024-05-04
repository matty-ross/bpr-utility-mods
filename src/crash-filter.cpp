#include "mem.h"


__declspec(naked) void UpdateEffects()
{
    static constexpr float blendAmount = 1.0f;
    static constexpr char wreckedEffectName[] = "Wrecked";
    static constexpr char crashedEffectName[] = "Crashed";
    
    __asm
    {
        pushfd
        pushad
        
        // ArbStateSharedInfo& lSharedInfo = edi
        
        // if (lSharedInfo.mpPlayerCrashInfo->mbWrecked)
        // {
        mov eax, dword ptr [edi + 0x34]
        cmp byte ptr [eax + 0x26], 0
        je _not_wrecked

        // this->mbPlayerWasWreckedThisCrash = true
        mov byte ptr [esi + 0x3DF], 1

        // BrnDirector::Camera::EnsureEffectIsPlaying(BrnDirector::Camera::Camera&, const BrnDirector::EffectInterface&, const char*, float)
        movss xmm3, [blendAmount]
        push offset wreckedEffectName
        mov edx, dword ptr [edi + 0x30] // lSharedInfo.mpEffectInterface
        lea ecx, [esi + 0x10]           // this->mCamera
        mov eax, 0x004D2700
        call eax
        add esp, 4

        // }
        jmp _end

        // else
        // {
    _not_wrecked :
        // BrnDirector::Camera::EnsureEffectIsPlaying(BrnDirector::Camera::Camera&, const BrnDirector::EffectInterface&, const char*, float)
        movss xmm3, [blendAmount]
        push offset crashedEffectName
        mov edx, dword ptr [edi + 0x30] // lSharedInfo.mpEffectInterface
        lea ecx, [esi + 0x10]           // this->mCamera
        mov eax, 0x004D2700
        call eax
        add esp, 4
        
        // }

    _end:
        popad
        popfd
        ret
    }
}

void Patch()
{
    // call UpdateEffects
    {
        static uint8_t code[] = { 0xE8, 0x00, 0x00, 0x00, 0x00 };
        *reinterpret_cast<uint32_t*>(code + 0x1) = reinterpret_cast<uint32_t>(UpdateEffects) - 0x0096F870 - 0x5;
        PatchCode(0x0096F870, sizeof(code), code);

        RemoveCode(0x0096F875, 11);
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
