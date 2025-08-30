#ifndef POKEMON_H
#define POKEMON_H

#define INCLUDE_IOSTREAM true
#define USE_EXTERNAL_DATA false
#define USE_CPP_RAND true

#include "typeDefs.h"
#include "pokemon_data.h"

#if INCLUDE_IOSTREAM
#include <iostream>
#include <sstream>
#endif

#if !USE_EXTERNALDATA
//#include "pokemonData.h"
#endif

#if USE_CPP_RAND

#endif

// Avoid having to import math
inline u32 sizeToMask(int len)
{
    u32 out = 1;
    for (int i = 0; i < len; i++)
    {
        out *= 2;
    }
    return (out - 1);
}

#if USE_CPP_RAND
// #include <random>
#endif

inline u32 getPureRand() // Gets a random number from the device itself
{
#if USE_CPP_RAND
    srand(time(0));
    return rand() << 16 | rand();
#else

#endif
}

// FNV-1a 32-bit hash function for byte arrays
inline u32 fnv1a_hash(unsigned char *data, size_t length)
{
    const uint32_t fnv_prime = 0x01000193;
    const uint32_t fnv_offset_basis = 0x811C9DC5;
    uint32_t hash = fnv_offset_basis;

    for (size_t i = 0; i < length; ++i)
    {
        hash ^= data[i];
        hash *= fnv_prime;
    }
    return hash;
}

class Pokemon // The base Pokemon class
{

public:
    // This is used to load our data in from an array
    void loadData(byte incomingArray[]);

#if INCLUDE_IOSTREAM
    virtual void print(std::ostream &os)
    {
        os << "This is a base Pokemon, it has no info!";
    };
    
    friend std::ostream &operator<<(std::ostream &os, Pokemon &p)
    {
        p.print(os);
        return os;
    }
#endif

protected:
    //  Will be set by child classes
    byte *dataArrayPtr;
    int dataArraySize;

    // These can probably move
    byte *nicknameArrayPtr;
    int nicknameArraySize;

    byte *OTArrayPtr;
    int OTArraySize;

    byte *externalIndexNumberPtr;
    
    bool isBigEndian;

    u32 getVar(DataVarInfo dataVar);
    u32 getVar(DataVarInfo dataVar, int extraByteOffset);

    bool setVar(DataVarInfo dataVar, u32 newValue);
    bool setVar(DataVarInfo dataVar, int extraByteOffset, u32 newValue);
};

#endif
