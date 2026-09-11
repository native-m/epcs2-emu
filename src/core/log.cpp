#include "log.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <iostream>
#include <cstdio>
#include <cstdarg>

namespace epcs2 {

static std::shared_ptr<spdlog::logger> s_logger;

void Log::initialize() {
    try {
        s_logger = spdlog::basic_logger_mt("epcs2", "epcs2.log");
    } catch (const spdlog::spdlog_ex &ex) {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
}

void Log::info(const char* fmt, ...) {
    va_list args;
    char buffer[256];
    va_start(args, fmt);
    std::vsnprintf(buffer, 256, fmt, args);
    va_end(args);
    s_logger->info(buffer);
}

void Log::debug(const char* fmt, ...) {
    va_list args;
    char buffer[256];
    va_start(args, fmt);
    std::vsnprintf(buffer, 256, fmt, args);
    va_end(args);
    s_logger->debug(buffer);
}

void Log::error(const char* fmt, ...) {
    va_list args;
    char buffer[256];
    va_start(args, fmt);
    std::vsnprintf(buffer, 256, fmt, args);
    va_end(args);
    s_logger->error(buffer);
}

}