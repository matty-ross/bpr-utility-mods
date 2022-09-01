#pragma once


#include <cstdint>

#include <Windows.h>


void PatchCode(uintptr_t address, size_t size, const void* code)
{
    void* addr = reinterpret_cast<void*>(address);
    
    DWORD oldProtection = 0;
    VirtualProtect(addr, size, PAGE_EXECUTE_READWRITE, &oldProtection);
    memcpy_s(addr, size, code, size);
    VirtualProtect(addr, size, oldProtection, &oldProtection);
}

void RemoveCode(uintptr_t address, size_t size)
{
    void* addr = reinterpret_cast<void*>(address);

    DWORD oldProtection = 0;
    VirtualProtect(addr, size, PAGE_EXECUTE_READWRITE, &oldProtection);
    memset(addr, 0x90, size);
    VirtualProtect(addr, size, oldProtection, &oldProtection);
}