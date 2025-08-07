#ifndef TYPEDEFS_H
#define TYPEDEFS_H

typedef unsigned char u8, byte, uchar, echar;
typedef unsigned short u16, hword, ushort, eshort;
typedef unsigned int u32, word, uint, eint;
typedef unsigned long long u64;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

// byte offset, data length (in bits), and bit offset
struct DataVarInfo
{
public:
    int byteOffset; // The offset of the value
    // std::string data_name; // The name of the data
    int dataLength; // The length of the data in bits
    int bitOffset;  // The offset of where the bits are in the array
    // void *data_array_ptr;  // A pointer to the data array (ie a list of Pokemon)
};

enum GetSet
{
    GET,
    SET,
};

enum Stat
{
    HP,
    ATTACK,
    DEFENSE,
    SPEED,
    SPECIAL,
    SPECIAL_ATTACK,
    SPECIAL_DEFENSE
};

#endif