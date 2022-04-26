#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define null ((void*)0x0)

#if defined(__clang__) || defined(__gcc__)
	#define force_inline __attribute__((always_inline)) inline
	#define dont_inline __attribute__((noinline))
#elif defined(_MSC_VER)
	#define force_inline __forceinline
	#define dont_inline __declspec(noinline)
#else
	#define force_inline static inline
	#define dont_inline
#endif

#define maximum(a_, b_) ((a_) > (b_) ? (a_) : (b_))
#define minimum(a_, b_) ((a_) < (b_) ? (a_) : (b_))

#define screen_w 1920
#define screen_h 1080

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef size_t usize;

typedef float f32;
typedef double f64;

bool read_raw(const char* filename, u8** buf, u64* size, bool term);
char* copy_string(const char* str);
char* get_file_path(const char* name);
u64 file_mod_time(const char* name);

void print_log(const char* fmt, ...);

f32 random_float(f32 min, f32 max);
