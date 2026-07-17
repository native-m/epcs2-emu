#pragma once

#include <cstdint>

namespace epcs2 {

struct MMU {
    static constexpr uintptr_t vm_base_address = 0x80000000ull;

    static constexpr size_t ee_main_memory_size = 32 * 1024 * 1024;
    static constexpr size_t io_registers_size = 64 * 1024;
    static constexpr size_t vu_memory_size = 64 * 1024;
    static constexpr size_t gs_registers_size = 8 * 1024;
    static constexpr size_t scratchpad_memory_size = 16 * 1024;

    static constexpr size_t ee_main_memory_offset0 = 0x00000000;
    static constexpr size_t io_registers_offset = 0x10000000;
    static constexpr size_t ee_main_memory_offset1 = 0x20000000;
    static constexpr size_t ee_main_memory_offset2 = 0x30000000;
    static constexpr size_t vu0_inst_memory_offset = 0x11000000;
    static constexpr size_t vu0_data_memory_offset = 0x11004000;
    static constexpr size_t vu1_inst_memory_offset = 0x11008000;
    static constexpr size_t vu1_data_memory_offset = 0x1100C000;
    static constexpr size_t gs_registers_offset = 0x12000000;
    static constexpr size_t scratchpad_memory_offset = 0x70000000;

    void* ee_main_memory_section;
    void* ee_main_memory0;
    void* ee_main_memory1;
    void* ee_main_memory2;
    void* io_registers;
    void* vu_memory;
    void* gs_registers;
    void* scratchpad_memory;

    MMU();
    ~MMU();

    inline uint8_t read8(uint32_t addr) { return *reinterpret_cast<uint8_t*>(vm_base_address + addr); }

    inline uint16_t read16(uint32_t addr) { return *reinterpret_cast<uint16_t*>(vm_base_address + addr); }

    inline uint32_t read32(uint32_t addr) { return *reinterpret_cast<uint32_t*>(vm_base_address + addr); }

    inline uint64_t read64(uint32_t addr) { return *reinterpret_cast<uint64_t*>(vm_base_address + addr); }

    inline void write8(uint32_t addr, uint8_t data) { *reinterpret_cast<uint8_t*>(vm_base_address + addr) = data; }

    inline void write16(uint32_t addr, uint16_t data) { *reinterpret_cast<uint16_t*>(vm_base_address + addr) = data; }

    inline void write32(uint32_t addr, uint32_t data) { *reinterpret_cast<uint32_t*>(vm_base_address + addr) = data; }

    inline void write64(uint32_t addr, uint64_t data) { *reinterpret_cast<uint64_t*>(vm_base_address + addr) = data; }

    void write(uint32_t start_addr, const void* src, size_t size);

    void erase(uint32_t start_addr, size_t size);

    static MMU self;
};

} // namespace epcs2