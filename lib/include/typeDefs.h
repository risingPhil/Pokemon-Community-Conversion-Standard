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

enum Condition
{
    COOLNESS,
    BEAUTY,
    CUTENESS,
    SMARTNESS,
    TOUGHNESS,
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

enum Ribbon
{
    COOL_NORMAL_CONTEST,
    COOL_SUPER_CONTEST,
    COOL_HYPER_CONTEST,
    COOL_MASTER_CONTEST,
    BEAUTY_NORMAL_CONTEST,
    BEAUTY_SUPER_CONTEST,
    BEAUTY_HYPER_CONTEST,
    BEAUTY_MASTER_CONTEST,
    CUTE_NORMAL_CONTEST,
    CUTE_SUPER_CONTEST,
    CUTE_HYPER_CONTEST,
    CUTE_MASTER_CONTEST,
    SMART_NORMAL_CONTEST,
    SMART_SUPER_CONTEST,
    SMART_HYPER_CONTEST,
    SMART_MASTER_CONTEST,
    TOUGH_NORMAL_CONTEST,
    TOUGH_SUPER_CONTEST,
    TOUGH_HYPER_CONTEST,
    TOUGH_MASTER_CONTEST,
    CHAMPION,
    WINNING,
    VICTORY,
    ARTIST,
    EFFORT,
    BATTLECHAMPION,
    REGIONALCHAMPION,
    NATIONALCHAMPION,
    COUNTRY,
    NATIONAL,
    EARTH,
};

enum Gender{
    MALE,
    FEMALE,
    GENDERLESS,
};

enum Nature{
    HARDY,
    LONELY,
    BRAVE,
    ADAMANT,
    NAUGHTY,
    BOLD,
    DOCILE,
    RELAXED,
    IMPISH,
    LAX,
    TIMID,
    HASTY,
    SERIOUS,
    JOLLY,
    NAIVE,
    MODEST,
    MILD,
    QUIET,
    BASHFUL,
    RASH,
    CALM,
    GENTLE,
    SASSY,
    CAREFUL,
    QUIRKY,
};

#endif