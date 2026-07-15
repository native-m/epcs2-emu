#pragma once

#include <cstdio>

// clang-format off
#define LOG_DEBUG(...)   {std::printf("[D] " __VA_ARGS__); std::printf("\n");}
#define LOG_VERBOSE(...) {std::printf("[V] " __VA_ARGS__); std::printf("\n");}
#define LOG_TRACE(...)   {std::printf("[T] " __VA_ARGS__); std::printf("\n");}
#define LOG_ERROR(...)   {std::printf("[E] " __VA_ARGS__); std::printf("\n");}
#define LOG_FATAL(...)   {std::printf("[F] " __VA_ARGS__); std::printf("\n");}
// clang-format on