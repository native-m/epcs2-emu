#include "sys_memory.h"

#include <cassert>
#include <cstring>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace epcs2 {

SysMemory::SysMemory() {
    ee_main_memory_section =
        CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, ee_main_memory_size, "epcs2_ee_memory");
    assert(ee_main_memory_section != nullptr);

    ee_main_memory0 = VirtualAlloc2(nullptr, (void*)(vm_base_address + ee_main_memory_offset0), ee_main_memory_size,
                                    MEM_RESERVE | MEM_RESERVE_PLACEHOLDER, PAGE_NOACCESS, nullptr, 0);
    assert(ee_main_memory0 != nullptr);
    VirtualFree(ee_main_memory0, 0, MEM_RELEASE | MEM_PRESERVE_PLACEHOLDER);

    ee_main_memory1 = VirtualAlloc2(nullptr, (void*)(vm_base_address + ee_main_memory_offset1), ee_main_memory_size,
                                    MEM_RESERVE | MEM_RESERVE_PLACEHOLDER, PAGE_NOACCESS, nullptr, 0);
    assert(ee_main_memory1 != nullptr);
    VirtualFree(ee_main_memory1, 0, MEM_RELEASE | MEM_PRESERVE_PLACEHOLDER);

    ee_main_memory2 = VirtualAlloc2(nullptr, (void*)(vm_base_address + ee_main_memory_offset2), ee_main_memory_size,
                                    MEM_RESERVE | MEM_RESERVE_PLACEHOLDER, PAGE_NOACCESS, nullptr, 0);
    assert(ee_main_memory2 != nullptr);
    VirtualFree(ee_main_memory2, 0, MEM_RELEASE | MEM_PRESERVE_PLACEHOLDER);

    ee_main_memory0 = MapViewOfFile3(ee_main_memory_section, nullptr, ee_main_memory0, 0, ee_main_memory_size,
                                     MEM_REPLACE_PLACEHOLDER, PAGE_READWRITE, nullptr, 0);
    assert(ee_main_memory0 != nullptr);

    ee_main_memory1 = MapViewOfFile3(ee_main_memory_section, nullptr, ee_main_memory1, 0, ee_main_memory_size,
                                     MEM_REPLACE_PLACEHOLDER, PAGE_READWRITE, nullptr, 0);
    assert(ee_main_memory1 != nullptr);

    ee_main_memory2 = MapViewOfFile3(ee_main_memory_section, nullptr, ee_main_memory2, 0, ee_main_memory_size,
                                     MEM_REPLACE_PLACEHOLDER, PAGE_READWRITE, nullptr, 0);
    assert(ee_main_memory2 != nullptr);

    io_registers = VirtualAlloc2(nullptr, (void*)(vm_base_address + io_registers_offset), io_registers_size,
                                 MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE, nullptr, 0);
    assert(io_registers != nullptr);

    vu_memory = VirtualAlloc2(nullptr, (void*)(vm_base_address + vu0_inst_memory_offset), vu_memory_size,
                              MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE, nullptr, 0);
    assert(vu_memory != nullptr);

    gs_registers = VirtualAlloc2(nullptr, (void*)(vm_base_address + gs_registers_offset), gs_registers_size,
                                 MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE, nullptr, 0);
    assert(gs_registers != nullptr);

    scratchpad_memory = VirtualAlloc2(nullptr, (void*)(vm_base_address + scratchpad_memory_offset),
                                      scratchpad_memory_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE, nullptr, 0);
    assert(scratchpad_memory != nullptr);
}

SysMemory::~SysMemory() {
    UnmapViewOfFile(ee_main_memory0);
    UnmapViewOfFile(ee_main_memory1);
    UnmapViewOfFile(ee_main_memory2);
    VirtualFree(io_registers, 0, MEM_RELEASE);
    VirtualFree(vu_memory, 0, MEM_RELEASE);
    VirtualFree(gs_registers, 0, MEM_RELEASE);
    VirtualFree(scratchpad_memory, 0, MEM_RELEASE);
    CloseHandle(ee_main_memory_section);
}

void SysMemory::write_data(uint32_t start_addr, const void* src, size_t size) {
    std::memcpy((void*)(vm_base_address + start_addr), src, size);
}

SysMemory SysMemory::self {};

} // namespace epcs2