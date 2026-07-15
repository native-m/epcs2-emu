#pragma once

#include "types.h"
#include <intrin.h>

namespace epcs2 {

inline bool add_overflow(const int32_t a, const int32_t b, int32_t& result) {
#if defined(__clang__) && defined(_MSC_VER)
    return __builtin_add_overflow(a, b, &result);
#elif defined(_MSC_VER)
    return (bool)_add_overflow_i32(0, a, b, &result);
#elif defined(__clang__)
    return __builtin_add_overflow(a, b, &result);
#else
    result = a + b;
    return !((a < 0 != b < 0) || (a < 0 == result < 0));
#endif
}

inline bool add_overflow(const int64_t a, const int64_t b, int64_t& result) {
#if defined(__clang__) && defined(_MSC_VER)
    return __builtin_add_overflow(a, b, &result);
#elif defined(_MSC_VER)
    return (bool)_add_overflow_i64(0, a, b, &result);
#elif defined(__clang__)
    return __builtin_add_overflow(a, b, &result);
#else
    result = a + b;
    return !((a < 0 != b < 0) || (a < 0 == result < 0));
#endif
}

inline bool sub_overflow(const int32_t a, const int32_t b, int32_t& result) {
#if defined(__clang__) && defined(_MSC_VER)
    return __builtin_sub_overflow(a, b, &result);
#elif defined(_MSC_VER)
    return (bool)_sub_overflow_i32(0, a, b, &result);
#elif defined(__clang__)
    return __builtin_sub_overflow(a, b, &result);
#else
    result = a - b;
    return ((a < 0 == b < 0) && (a < 0 != result < 0));
#endif
}

inline bool sub_overflow(const int64_t a, const int64_t b, int64_t& result) {
#if defined(__clang__) && defined(_MSC_VER)
    return __builtin_sub_overflow(a, b, &result);
#elif defined(_MSC_VER)
    return (bool)_sub_overflow_i64(0, a, b, &result);
#elif defined(__clang__)
    return __builtin_sub_overflow(a, b, &result);
#else
    result = a - b;
    return ((a < 0 == b < 0) && (a < 0 != result < 0));
#endif
}

} // namespace epcs2