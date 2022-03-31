#pragma once

#include <stdint.h>
#include <stdbool.h>

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

#define screen_w 1366
#define screen_h 768

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

bool read_raw(const char* filename, u8** buf, u64* size, bool term);
char* copy_string(const char* str);
char* get_file_path(const char* name);
u64 elf_hash(const u8* data, u32 size);
