cmake_minimum_required (VERSION 3.22)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("epcs2-emu")

include(Dependencies.cmake)

add_library(epcs2-emu-lib
    "src/core/intrinsics.h"
    "src/core/log.cpp"
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

target_link_libraries(epcs2-emu-lib PUBLIC
    SDL3::SDL3-static
    spdlog::spdlog
    slang::slang
    slang::slang-compiler)

if (WIN32)
    target_link_libraries(epcs2-emu-lib PUBLIC onecore.lib)
endif()

add_executable(epcs2-emu "src/app.cpp")
target_link_libraries(epcs2-emu PRIVATE epcs2-emu-lib)
set_property(TARGET epcs2-emu PROPERTY CXX_STANDARD 20)

# copy slang binaries
add_custom_command(TARGET epcs2-emu POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SLANG_RUNTIME}"
        "$<TARGET_FILE_DIR:epcs2-emu>")
add_custom_command(TARGET epcs2-emu POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SLANG_COMPILER_RUNTIME}"
        "$<TARGET_FILE_DIR:epcs2-emu>")

add_subdirectory(test)