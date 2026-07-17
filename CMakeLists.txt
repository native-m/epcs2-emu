cmake_minimum_required (VERSION 3.22)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("epcs2-emu")

add_library(epcs2-emu-lib
    "src/core/intrinsics.h"
    "src/core/log.h"
    "src/core/os.cpp"
    "src/core/os.h"
    "src/core/types.h"
    "src/mmu.h"
    "src/mmu.cpp"
    "src/r5900_regs.h"
    "src/r5900_vcpu.cpp"
    "src/r5900_vcpu.h")

target_include_directories(epcs2-emu-lib PUBLIC "${CMAKE_SOURCE_DIR}/src")
set_property(TARGET epcs2-emu-lib PROPERTY CXX_STANDARD 20)

if (WIN32)
    target_link_libraries(epcs2-emu-lib PUBLIC onecore.lib)
endif()

add_executable(epcs2-emu "src/app.cpp")

add_subdirectory(test)