#include "log.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <cstdarg>
#include <cstdio>
#include <iostream>

namespace epcs2 {

static std::shared_ptr<spdlog::logger> s_logger;

void Log::initialize() {
    try {
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("epcs2.log", true);
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        s_logger = std::make_shared<spdlog::logger>("epcs2", spdlog::sinks_init_list {file_sink, console_sink});
    } catch (const spdlog::spdlog_ex& ex) {
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

} // namespace epcs2