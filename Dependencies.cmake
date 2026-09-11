include(cmake/CPM.cmake)

CPMAddPackage(
    NAME                SDL
    GITHUB_REPOSITORY   libsdl-org/SDL
    VERSION             3.4.12
    GIT_TAG             release-3.4.12
    OPTIONS             "SDL_SHARED OFF"
                        "SDL_STATIC ON"
                        "SDL_TESTS OFF"
                        "SDL_TEST_LIBRARY OFF"
                        "SDL_DISABLE_INSTALL ON"
                        "SDL_DISABLE_INSTALL_DOCS ON"
                        "SDL_INSTALL_TESTS ON"
                        # Disable unused subsystems
                        "SDL_ATOMIC OFF"
                        "SDL_AUDIO OFF"
                        "SDL_RENDER OFF"
                        "SDL_RENDER_GPU OFF"
                        "SDL_JOYSTICK ON"
                        "SDL_HAPTIC OFF"
                        "SDL_OPENGLES OFF"
                        "SDL_GPU OFF"
                        "SDL_CAMERA OFF"
    EXCLUDE_FROM_ALL
)

CPMAddPackage(
    NAME                spdlog
    VERSION             1.17.0
    GITHUB_REPOSITORY   gabime/spdlog
    OPTIONS             "SPDLOG_NO_EXCEPTIONS OFF"
                        "SPDLOG_FMT_EXTERNAL OFF"
    EXCLUDE_FROM_ALL
)

CPMAddPackage(
    NAME slang
    URL https://github.com/shader-slang/slang/releases/download/v2026.17/slang-2026.17-windows-x86_64.zip
    URL_HASH SHA256=bc8cf08b24aaf44d98f06b7d578d0557a21bfed1ce6bfb9765a0f0d21dec8f31
    DOWNLOAD_ONLY YES
)

if (slang_ADDED)
    find_library(SLANG_LIBRARY
        NAMES slang
        PATHS "${slang_SOURCE_DIR}/lib"
        NO_DEFAULT_PATH
        REQUIRED)

    find_file(SLANG_RUNTIME
        NAMES slang.dll
        PATHS "${slang_SOURCE_DIR}/bin"
        NO_DEFAULT_PATH
        REQUIRED)

    add_library(slang::slang SHARED IMPORTED GLOBAL)
    
    set_target_properties(slang::slang PROPERTIES
        IMPORTED_IMPLIB "${SLANG_LIBRARY}"
        IMPORTED_LOCATION "${SLANG_RUNTIME}"
        INTERFACE_INCLUDE_DIRECTORIES "${slang_SOURCE_DIR}/include")

    find_library(SLANG_COMPILER_LIBRARY
        NAMES slang-compiler
        PATHS "${slang_SOURCE_DIR}/lib"
        NO_DEFAULT_PATHS
        REQUIRED)

    find_file(SLANG_COMPILER_RUNTIME
        NAMES slang-compiler.dll
        PATHS "${slang_SOURCE_DIR}/bin"
        NO_DEFAULT_PATH
        REQUIRED)

    add_library(slang::slang-compiler SHARED IMPORTED GLOBAL)

    set_target_properties(slang::slang-compiler PROPERTIES
        IMPORTED_IMPLIB "${SLANG_COMPILER_LIBRARY}"
        IMPORTED_LOCATION "${SLANG_COMPILER_RUNTIME}"
        INTERFACE_INCLUDE_DIRECTORIES "${slang_SOURCE_DIR}/include")
endif()
