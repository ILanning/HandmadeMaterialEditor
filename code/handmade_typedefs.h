#ifndef HANDMADE_TYPEDEFS
#define HANDMADE_TYPEDEFS

#define internal static 
#define local_persist static 
#define global_variable static

//TODO(Ian): Max/Min Real32/64

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

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

#endif