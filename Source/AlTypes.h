#pragma once

#include <climits>

namespace Al
{
    #define UNDEFINED_ROOM_ID 0xff
    #define UNDEFINED_OBJECT_ID 0xff

    #define MAX_U8 UCHAR_MAX
    #define MAX_U16 USHRT_MAX
    #define MAX_U32 UINT_MAX
    #define MAX_U64 ULLONG_MAX

    typedef unsigned char u8;
    typedef unsigned short u16;
    typedef unsigned int u32;
    typedef unsigned long long u64;
}
