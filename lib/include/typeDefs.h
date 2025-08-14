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
    int dataLength; // The length of the data in bits
    int bitOffset;  // The offset of where the bits are in the array
};

enum Stat
{
    HP,
    ATTACK,
    DEFENSE,
    SPEED,
    SPECIAL_ATTACK,
    SPECIAL_DEFENSE,
    SPECIAL = SPECIAL_ATTACK,
};

enum Gen1Types
{
    NORMAL,
    FIGHTING,
    FLYING,
    POISON,
    GROUND,
    ROCK,
    BIRD,
    BUG,
    GHOST,
    NINE,
    TEN,
    ELEVEN,
    TWELVE,
    THIRTEEN,
    FOURTEEN,
    FIFTEEN,
    SIXTEEN,
    SEVENTEEN,
    EIGHTEEN,
    NINETEEN,
    FIRE,
    WATER,
    GRASS,
    ELECTRIC,
    PSYCHIC,
    ICE,
    DRAGON,
};

#endif