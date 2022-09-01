#include "mem.h"


const char* GetManufacturer(uint64_t vehicleID)
{
    const char* manufacturer = nullptr;

    __asm
    {
        push dword ptr [vehicleID + 0x4]
        push dword ptr [vehicleID + 0x0]

        mov ecx, dword ptr ds:[0x013FC8E0]
        add ecx, 0x68C350

        mov eax, 0x004B7080
        call eax

        test eax, eax
        je done 
        add eax, 0x70
    
    done:
        mov dword ptr [manufacturer], eax
    }

    return manufacturer;
}

int32_t __cdecl GetVehicleManufacturer(uint64_t vehicleID)
{
    static constexpr const char* const manufacturers[] =
    {
        "Carson",
        "Hunter",
        "Jansen",
        "Krieger",
        "Kitano",
        "Montgomery",
        "Nakamura",
        "Rossolini",
        "Watson"
    };

    const char* const manufacturer = GetManufacturer(vehicleID);
    if (manufacturer != nullptr)
    {
        for (int i = 0; i < 9; ++i)
        {
            if (_stricmp(manufacturer, manufacturers[i]) == 0)
            {
                return i;
            }
        }
    }

    return 9;
}

void Patch()
{
    // redirect function
    {
        const uintptr_t dest = reinterpret_cast<uintptr_t>(GetVehicleManufacturer) - 0x00A69A20 - 0x5;
        PatchCode(0x00A69A21, sizeof(dest), &dest);
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
