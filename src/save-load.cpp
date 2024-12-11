#include "../mem.h"


__declspec(naked) void SaveLoadHandler()
{
    static const char saveLoadAction[] = "TO_PROFILE";

    __asm
    {
        push offset saveLoadAction
        mov ecx, esi
        mov eax, 0x00894D50
        call eax
        
        pop esi
        mov esp, ebp
        pop ebp
        ret
    }
}

__declspec(naked) void AddMenuItem()
{
    __asm
    {
        mov eax, dword ptr [ebx + 0xF98]
        mov dword ptr [ebx + eax * 8 + 0xFA0], 7
        mov dword ptr [ebx + eax * 8 + 0xFA4], 0
        inc dword ptr [ebx + 0xF98]

        // original code
        push dword ptr [ebx + 0xF98]
        push 0x00B239BB
        ret
    }
}

void Patch()
{
    // change the number of menu items
    {
        static const uint8_t code[] = { 0x07 };
        PatchCode(0x00AFB5CA, sizeof(code), code);
    }

    // change menu item handlers
    {
        static const uint32_t cases[] =
        {
            reinterpret_cast<uint32_t>(SaveLoadHandler),
            0x00AFB5D8, // video options
            0x00AFB5E9, // audio options
            0x00AFB5FA, // control options
            0x00AFB60B, // game options
            0x00AFB66D, // roll credits
            0x00AFB61C, // back to main menu
            0x00AFB64C, // save & quit
        };
        const void* helper = cases;
        PatchCode(0x00AFB5D4, 4, &helper);
    }

    // change menu item titles
    {
        static const char* titles[]
        {
            "$SettingMenu_1",
            reinterpret_cast<const char*>(0x00E1AC40), // $CN_TITLE_OPT_VIDEO
            reinterpret_cast<const char*>(0x00E1AC2C), // $CN_TITLE_OPT_AUDIO
            reinterpret_cast<const char*>(0x00E1AC68), // $CN_TITLE_OPT_CONTROLS
            reinterpret_cast<const char*>(0x00E1AC54), // $CN_TITLE_OPT_GAME
            reinterpret_cast<const char*>(0x00E1AC04), // $SettingMenu_4
            reinterpret_cast<const char*>(0x00E1ABE0), // $MENU_OPTIONS_RETURN_TO_MAIN_MENU
            reinterpret_cast<const char*>(0x00E1AC1C), // $SettingMenu_9
        };
        const void* helper = titles;
        PatchCode(0x0767CC19, 4, &helper);
    }

    // add menu item
    {
        static uint8_t code[] = { 0xE9, 0x00, 0x00, 0x00, 0x00, 0x90 };
        *reinterpret_cast<uint32_t*>(code + 0x1) = reinterpret_cast<uint32_t>(AddMenuItem) - 0x00B239B5 - 0x5;
        PatchCode(0x00B239B5, sizeof(code), code);
    }

    // adjust disable 'roll credits' menu item index
    {
        uint8_t index = 5;
        PatchCode(0x0767CBB1, sizeof(index), &index);
    }
    
    // adjust disable 'back to main menu' menu item index
    {
        uint8_t index = 6;
        PatchCode(0x0767CBD5, sizeof(index), &index);
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