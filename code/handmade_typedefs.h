#ifndef HANDMADE_TYPEDEFS_H
#define HANDMADE_TYPEDEFS_H

#define internal static 
#define local_persist static 
#define global_variable static

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

#include <stdint.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;

#define Pi32 3.14159265359f

#define MaxInt16 0x7fff
#define MinInt16 0x8000
#define MaxInt32 0x7fffffff
#define MinInt32 0x80000000
#define MaxInt64 0x7fffffffffffffff
#define MinInt64 0x8000000000000000

#define MaxUInt16 0xffff
#define MinUInt16 0x0000
#define MaxUInt32 0xffffffff
#define MinUInt32 0x00000000
#define MaxUInt64 0xffffffffffffffff
#define MinUInt64 0x0000000000000000
//TODO: Max/Min Real32/64

#endif //HANDMADE_TYPEDEFS_H