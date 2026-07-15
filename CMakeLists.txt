cmake_minimum_required (VERSION 3.22)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("epcs2-emu")

add_executable(epcs2-emu
    "src/core/intrinsics.h"
    "src/core/os.cpp"
    "src/core/os.h"
    "src/core/types.h"
    "src/app.cpp"
    "src/r5900_regs.h"
    "src/r5900_vcpu.cpp"
    "src/r5900_vcpu.h"
    "src/sys_memory.h"
    "src/sys_memory.cpp")

if (WIN32)
    target_link_libraries(epcs2-emu PRIVATE onecore.lib)
endif()

set_property(TARGET epcs2-emu PROPERTY CXX_STANDARD 20)
